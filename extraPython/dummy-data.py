import serial
import time

# Configure the serial port
ser = serial.Serial('COM20', 9600)  # Replace with your COM port and baud rate

# Distance
distance = 50  # 50 centimeters

try:
    # Move from 90 degrees to 0 degrees
    for angle in range(90, -1, -1):
        # Construct the data string
        data_string = f"{angle},{distance},{distance}\n"
        
        # Write the data to serial
        ser.write(data_string.encode())
        
        # Delay for a short time
        time.sleep(0.1)  # Adjust delay time as needed
    
    # Move from 0 degrees back to 90 degrees
    for angle in range(1, 91):
        # Construct the data string
        data_string = f"{angle},{distance},{distance}\n"
        
        # Write the data to serial
        ser.write(data_string.encode())
        
        # Delay for a short time
        time.sleep(0.1)  # Adjust delay time as needed

finally:
    # Close the serial port
    ser.close()
