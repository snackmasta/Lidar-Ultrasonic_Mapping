import serial
import time

# Configure the serial connection
ser = serial.Serial('COM9', 9600, timeout=1)

# Open the log file in append mode
with open('./log.txt', 'a') as log_file:
    while True:
        try:
            # Read a line of data from the serial port
            line = ser.readline().decode('utf-8').strip()
            if line:
                # Split the line into components
                parts = line.split(',')
                if len(parts) == 4:
                    angle, sonar1, sonar2, sonar3 = parts
                    # Log the data to the file
                    log_file.write(f"{angle},{sonar1},{sonar2},{sonar3}\n")
                    # Print to console (optional)
                    print(f"Logged: {angle}, {sonar1}, {sonar2}, {sonar3}")
            # Add a small delay to avoid overwhelming the CPU
            time.sleep(0.1)
        except KeyboardInterrupt:
            # Exit the loop if interrupted by the user
            print("Logging stopped by user.")
            break
        except Exception as e:
            # Handle any other exceptions
            print(f"Error: {e}")

# Close the serial connection
ser.close()
