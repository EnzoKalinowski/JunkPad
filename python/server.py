import serial
import pyautogui
import time
ser = serial.Serial('COM5', 9600)


macros = {
    "MACRO_1_ON":  ['win', 'alt', 'shift', 'f12'],
    "MACRO_1_OFF": ['win', 'alt', 'shift', 'f11']
}
def hotkeys(keys):
    for key in keys:
        pyautogui.keyDown(key)
        time.sleep(0.05)
    for key in reversed(keys):
        pyautogui.keyUp(key)

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

