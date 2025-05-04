from gpiozero import LED
import sounddevice as sd
import queue
import vosk
import json

led = LED(17)
audio_queue = queue.Queue()
model = vosk.Model("vosk-model-small-en-us-0.15")
recognizer = vosk.KaldiRecognizer(model, 16000)


def callback(indata, frames, time, status):
    audio_queue.put(bytes(indata))


def activate_listening():
    while True:
        audio_data = audio_queue.get()
        if recognizer.AcceptWaveform(audio_data):
            result = json.loads(recognizer.Result())
            text = result.get("text", "").lower()

            if text == "raspberry":
                print("How can I help?")
                return True

            elif text == "that is all":
                print("Glad I could help!")
                return False


def execution():
    print("Starting microphone...")

    with sd.RawInputStream(samplerate=16000, blocksize=8000, dtype='int16',
                           channels=1, callback=callback):
        print("Ready. Say 'Raspberry' to begin...")

        while activate_listening():
            while True:
                audio_data = audio_queue.get()
                if recognizer.AcceptWaveform(audio_data):
                    result = json.loads(recognizer.Result())
                    text = result.get("text", "").lower()
                    print("You said:", text)

                    if "turn on green light" in text:
                        led.on()
                        print("LED turned ON")

                    elif "turn off green light" in text:
                        led.off()
                        print("LED turned OFF")

                    elif "that is all" in text:
                        print("Glad I could help!")
                        exit()


execution()
