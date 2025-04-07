import RPi.GPIO as GPIO
import tkinter as tk
from tkinter import messagebox

# Setup GPIO for LEDs and PWM
GPIO.setmode(GPIO.BOARD)
GPIO.setup(33, GPIO.OUT)  # Green
GPIO.setup(32, GPIO.OUT)  # Yellow
GPIO.setup(35, GPIO.OUT)  # Red

green_pwm = GPIO.PWM(33, 1000)
yellow_pwm = GPIO.PWM(32, 1000)
red_pwm = GPIO.PWM(35, 1000)

green_pwm.start(0)
yellow_pwm.start(0)
red_pwm.start(0)


class LEDControl:
    def __init__(self, root):
        self.red_pressed = False
        self.yellow_pressed = False
        self.green_pressed = False
        self.root = root

    def adjust_green_brightness(self, value):
        green_pwm.ChangeDutyCycle(float(value))

    def adjust_yellow_brightness(self, value):
        yellow_pwm.ChangeDutyCycle(float(value))

    def adjust_red_brightness(self, value):
        red_pwm.ChangeDutyCycle(float(value))

    def exit_program(self):
        self.root.quit()


# Handle window close
def on_closing():
    green_pwm.stop()
    yellow_pwm.stop()
    red_pwm.stop()
    GPIO.cleanup()
    root.destroy()

def slider_green(root, led_control):
    tk.Label(root, text = "Green LED Brightness").pack(pady=10)
    green_slider = tk.Scale(root, from_ = 0, to = 100, orient = "horizontal", command = led_control.adjust_green_brightness)
    green_slider.set(0)
    green_slider.pack(pady=10)

def slider_yellow(root, led_control):
    tk.Label(root, text = "Yellow LED Brightness").pack(pady=10)
    yellow_slider = tk.Scale(root, from_ = 0, to = 100, orient="horizontal", command = led_control.adjust_yellow_brightness)
    yellow_slider.set(0)
    yellow_slider.pack(pady=10)

def slider_red(root, led_control):
    tk.Label(root, text = "Red LED Brightness").pack(pady=10)
    red_slider = tk.Scale(root, from_ = 0, to = 100, orient = "horizontal", command = led_control.adjust_red_brightness)
    red_slider.set(0)
    red_slider.pack(pady=10)

def exit_button(root, led_control):
    tk.Button(root, text = "Exit", width = 10, bg = "grey", command = led_control.exit_program).pack(pady=20)

def execution():
    root = tk.Tk()
    root.title("LED Control Panel")
    root.geometry("350x400")

    led_control = LEDControl(root)

    slider_green(root, led_control)
    slider_yellow(root, led_control)
    slider_red(root, led_control)

    exit_button(root, led_control)

    root.protocol("WM_DELETE_WINDOW", on_closing)

    root.mainloop()


execution()
