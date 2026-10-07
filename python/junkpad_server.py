from xmlrpc import client

import serial
import keyboard
import time
import json
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
from ctypes import cast, POINTER
from comtypes import CLSCTX_ALL
import threading
from obswebsocket import obsws, requests
import subprocess
import logging
from serial.tools import list_ports


logging.basicConfig(level=logging.DEBUG, format='%(asctime)s - %(levelname)s - %(message)s')

DEBUG = False
RETRY_INTERVAL = 3  # seconds
HANDSHAKE_QUERY = b"ID_REQUEST\n" 
HANDSHAKE_EXPECTED = b"JUNKPAD"
HANDSHAKE_TIMEOUT = 1.0


ser = None
led_states = {}

def set_led(led, mode):
    logging.debug(f"[led] set_led({led!r}, {mode!r}) ser={ser}")
    if not led or ser is None:
        return
    if mode == 'toggle':
        value = 0 if led_states.get(led, 0) else 1
    else:
        value = {'off': 0, 'on': 1, 'blink': 2}.get(mode)
        if value is None:
            logging.warning(f"[led] unknown mode: {mode}")
            return
    led_states[led] = value
    try:
        ser.write(f"{led}:{value}\n".encode())
    except serial.SerialException:
        logging.warning("[led] write failed")

def resend_led_states():
    for led, value in led_states.items():
        ser.write(f"{led}:{value}\n".encode())

def find_junkpad_port(baudrate: int) -> str | None:

    logging.debug("[serial] scanning ports...")
 
    for p in list_ports.comports():
        logging.debug(f"[serial] checking port {p.device}...")
        try:
            with serial.Serial(p.device, baudrate, timeout=HANDSHAKE_TIMEOUT) as candidate:
                time.sleep(2)  # let the board finish its reset-on-connect cycle
                candidate.reset_input_buffer()
                candidate.write(HANDSHAKE_QUERY)
                reply = candidate.readline().strip()
                logging.debug(f"[serial] received reply: {reply}")
                if reply.startswith(HANDSHAKE_EXPECTED):
                    logging.info(f"[serial] found Junkpad device on {p.device}")
                    return p.device
        except serial.SerialException:
            logging.debug(f"[serial] failed to communicate with {p.device}")
            continue  # port busy or unreadable, skip it
 
    return None

def connect_serial(port: str, baudrate: int) -> serial.Serial:
    first_attempt = True
    while True:
        try:
            ser = serial.Serial(port, baudrate, timeout=1)
            if not first_attempt:
                logging.info(f"[serial] reconnected on {port}")
            return ser
        except serial.SerialException:
            if first_attempt:
                logging.info(f"[serial] {port} not available, waiting for device...")
                first_attempt = False
            time.sleep(RETRY_INTERVAL)

def init_obs_websocket(host: str, port: int, password: str) :
    global obsws_client 
    obsws_client = obsws(host, port, password)
    try:
        obsws_client.connect()
        logging.debug("[obs-websocket] connected")
        return obsws_client
    except Exception as e:
        logging.error(f"[obs-websocket] failed to connect: {e}")
        return None

def init_audio():
    global win_volume
    devices = AudioUtilities.GetSpeakers()
    interface = devices._dev.Activate(   # <-- accès à l'objet COM brut via _dev
        IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
    win_volume = cast(interface, POINTER(IAudioEndpointVolume))


macros = {}

def read_config(file_path):
    with open(file_path, 'r') as file:
        config = json.load(file)
        logging.debug(f"[config] loaded configuration from {file_path}")
    return config

def load_macros_from_config(config):
    macros = {}
    for button in config['config']:
        btype = button['button_type']

        if btype == 'toggle_button':
            for action in button['actions']:
                suffix = 'ON' if action.get('is_on') else 'OFF'
                key = f"{button['id']}_{suffix}"
                macros.setdefault(key, []).append(action)

        elif btype in ('momentary', 'analog'):
            macros.setdefault(button['id'], []).extend(button['actions'])

    logging.debug(f"[config] loaded {len(macros)} macros from configuration")
    return macros

def run_hotkey(keys, hold_ms=50):
    logging.debug(f"[hotkey] running hotkey with keys: {keys}")
    for key in keys:
        keyboard.press(key)
        time.sleep(0.01)  #short pause between keys
    time.sleep(hold_ms / 1000)  #hold the keys for the specified duration
    for key in reversed(keys): #release the keys in reverse order
        keyboard.release(key)

def run_obs_websocket(request):
    logging.debug(f"[obs-websocket] sending request: {request}")
    obsws_client.call(request)

def run_analog_control(action):
    logging.debug(f"[analog-control] setting volume to: {action['target']}")
    set_volume(action['target'])

def run_launch_app(app_path):
    logging.debug(f"[launch-app] launching application: {app_path}")
    subprocess.Popen(app_path)


def set_volume(level):
    win_volume.SetMasterVolumeLevelScalar(level, None)





def main (port:str='AUTO', baudrate:int=9600):
    global ser
    if port == 'AUTO':
        port = find_junkpad_port(baudrate)
        if not port:
            logging.error("No Junkpad device found.")
            return
    
    logging.info("Loading configuration...")
    config = read_config('./config/config_keypad.json')
    macros = load_macros_from_config(config)

    ser = connect_serial(port, baudrate)
    connected_led = config.get('status_leds', {}).get('connected')
    set_led(connected_led, 'on')
    #init_obs_websocket('localhost', 4455, 'password')  # Adjust host, port, and password as needed
    init_audio()

    try:
    
        while True:
            try:
                logging.debug("[serial] waiting for data...")
                data = ser.readline().decode(errors="ignore").strip()
                logging.debug(f"[serial] received raw data: {data}")
            except serial.SerialException:
                logging.warning(f"[serial] lost connection on {port}, attempting to reconnect...")
                ser.close()
                ser = connect_serial(port, baudrate)
                set_led(connected_led, 'on')
                resend_led_states()
                continue

            if not data:
                continue 
            else:                
                logging.debug(f"[serial] received data: {data}")

            if ':' in data:
                btn_id, payload = data.split(':', 1)
            else:
                btn_id, payload = data, None

            if btn_id in macros:
                for action in macros[btn_id]:
                    atype = action.get('action_type')
                    if atype == 'hotkey':
                        threading.Thread(target=run_hotkey, args=(action['keys'],), daemon=True).start()
                    elif atype == 'launch_app':
                        run_launch_app(action['path'])
                    elif atype == 'obs_websocket':
                        request_cls = getattr(requests, action['request'], None)
                        if request_cls:
                            run_obs_websocket(request_cls())
                        else:
                            logging.warning(f"Unknown OBS request: {action['request']}")
                    elif atype == 'analog_control':
                        run_analog_control(action, int(payload))
                    elif atype == 'led':
                        set_led(action['led'], action.get('mode', 'toggle'))
                        logging.debug(f"[led] set {action['led']} to {action.get('mode', 'toggle')}")
                    else:
                        logging.debug(f"Unknown action type: {atype}")


    except KeyboardInterrupt:
        logging.info("Server stopped.")
    finally:
        logging.info("Cleaning up...")
        obsws_client.disconnect()
        set_led(connected_led, 'off')
        ser.close()


if __name__ == "__main__":
    main()
