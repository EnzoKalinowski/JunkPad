import serial
import pyautogui
import time



ser = serial.Serial('COM5', 9600)


macros = {
    "MACRO_1_ON": lambda: pyautogui.hotkey('win', 'alt','shift', 'f12'),
    "MACRO_1_OFF": lambda: pyautogui.hotkey('win', 'alt','shift', 'f11')


}

pyautogui.keyDown('win')
time.sleep(0.05)
pyautogui.keyDown('alt')
time.sleep(0.05)
pyautogui.keyDown('shift')
time.sleep(0.05)
pyautogui.keyDown('f12')
time.sleep(0.1)

pyautogui.keyUp('f12')
pyautogui.keyUp('shift')
pyautogui.keyUp('alt')
pyautogui.keyUp('win')

