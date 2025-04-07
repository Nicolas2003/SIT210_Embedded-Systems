import RPi.GPIO as GPIO
import tkinter as tk
from tkinter import messagebox

# Setup GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)  # Red
GPIO.setup(13, GPIO.OUT)  # Yellow
GPIO.setup(33, GPIO.OUT)  # Green


class LEDControl:
    def __init__(self, root):
        self.red_pressed = False
        self.yellow_pressed = False
        self.green_pressed = False
        self.root = root

    def green_led(self):
        if self.green_pressed == False:
            GPIO.output(33, GPIO.HIGH)
            self.green_pressed = True
        else:
            GPIO.output(33, GPIO.LOW)
            self.green_pressed = False

    def yellow_led(self):
        if self.yellow_pressed == False:
            GPIO.output(13, GPIO.HIGH)
            self.yellow_pressed = True
        else:
            GPIO.output(13, GPIO.LOW)
            self.yellow_pressed = False

    def red_led(self):
        if self.red_pressed == False:
            GPIO.output(11, GPIO.HIGH)
            self.red_pressed = True
        else:
            GPIO.output(11, GPIO.LOW)
            self.red_pressed = False

    def exit_program(self):
        self.root.quit()


def on_closing():
    GPIO.cleanup()
    root.destroy()


def execution():
    root = tk.Tk()
    root.title("LED Control Panel")
    root.geometry("300x300")

    led_control = LEDControl(root)

    tk.Button(root, text="Green", width=10, bg="green", command=led_control.green_led).pack(pady=10)
    tk.Button(root, text="Yellow", width=10, bg="yellow", command=led_control.yellow_led).pack(pady=10)
    tk.Button(root, text="Red", width=10, bg="red", command=led_control.red_led).pack(pady=10)

    tk.Button(root, text="Exit", width=10, bg="grey", command=led_control.exit_program).pack(pady=20)

    root.protocol("WM_DELETE_WINDOW", on_closing)
    root.mainloop()


execution()
