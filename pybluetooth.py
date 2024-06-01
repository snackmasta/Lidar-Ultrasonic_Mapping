import serial
import time

# Replace with your ESP32's Bluetooth serial port (e.g., COM16)
bluetooth_port = 'COM16'  # Replace with your actual COM port number

# Open the serial port connection
try:
    ser = serial.Serial(bluetooth_port, 9600, timeout=1)
    print(f"Connected to {bluetooth_port}")

    while True:
        # Read the incoming data from the ESP32
        if ser.in_waiting > 0:
            response = ser.readline().decode().strip()
            if response:
                print(f"Received: {response}")

        time.sleep(0.1)  # Delay to allow ESP32 to process data

except serial.SerialException as e:
    print(f"Serial port error: {e}")

finally:
    if ser.is_open:
        ser.close()
        print("Serial port closed.")