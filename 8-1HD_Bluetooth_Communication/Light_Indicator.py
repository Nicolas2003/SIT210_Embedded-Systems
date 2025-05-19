import asyncio
from bleak import BleakClient, BleakScanner
from gpiozero import LED

SERVICE_UUID = "a1b2c3d4-1234-4abc-8def-0123456789ab"
CHARACTERISTIC_UUID = "b2c3d4e5-2345-4bcd-9f01-123456789abc"

led = LED(16)

async def find_nano_device():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover()

    for d in devices:
        if d.name and "Nano33BLE" in d.name:
            print(f"Found Nano33BLE: {d.name} ({d.address})")
            return d

    print("Nano33BLE not found.")
    return None


async def main():
    nano_device = await find_nano_device()

    if not nano_device:
        return

    async with BleakClient(nano_device.address, timeout=500) as client:

        def closing_distance(sending, data):
            try:
                distance = float(data.decode().strip())
                print(f"Distance: {distance:.2f}cm")

                if distance < 10.0:
                    print("Closing distance")
                    led.on()
                else:
                    print("Keep coming, still got a few more centimeters")
                    led.off()
            except Exception as error:
                print("Error decoding bluetooth communication:", error)

        print("Connected. Starting notifications...")
        await client.start_notify(CHARACTERISTIC_UUID, closing_distance)

        print("Listening for notifications... Press Ctrl+C to stop.")
        try:
            while True:
                await asyncio.sleep(1)
        except KeyboardInterrupt:
            print("Stopping notifications...")

        await client.stop_notify(CHARACTERISTIC_UUID)
        led.off()
        print("Stopped notifications.")


asyncio.run(main())
