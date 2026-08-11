import serial
import pyautogui
import time
import json
ser = serial.Serial('COM5', 9600)


macros = {
    # "MACRO_1_ON":  ['win', 'alt', 'shift', 'f12'],
    # "MACRO_1_OFF": ['win', 'alt', 'shift', 'f11']
}

def read_config(file_path):
    with open(file_path, 'r') as file:
        config = json.load(file)
    return config

def load_macros_from_config(config):
    macros = {}
    for button in config['config']:
        for action in button['actions']:
            if button['type'] == 'locking_button':
                if action['is_on']:
                    macros[f"{button['name']}_ON"] = action['keys']
                else:
                    macros[f"{button['name']}_OFF"] = action['keys']
    return macros

def hotkeys(keys):
    for key in keys:
        pyautogui.keyDown(key)
        time.sleep(0.05)
    for key in reversed(keys):
        pyautogui.keyUp(key)

print("Loading configuration...")
config = read_config('./config/config_1.json')
macros = load_macros_from_config(config)

print("JunkPad Server running. Waiting for signals...")


try:
    while True:
        if ser.in_waiting:
            data = ser.readline().decode().strip()
            print(f"Received: {data}")
            if data in macros:
                hotkeys(macros[data])  # Exécuter la macro correspondante
except KeyboardInterrupt:
    print("Server stopped.")
    ser.close()

