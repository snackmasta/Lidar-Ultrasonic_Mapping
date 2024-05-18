import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
import time

# Constants
MAX_DISTANCE = 200

# Setup the serial port connection
ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(5)  # Allow time for the connection to establish

angles = []
distances = []

# Function to update the plot
def update(frame):
    if ser.in_waiting > 0:
        try:
            data = ser.readline().decode('utf-8', errors='ignore').strip()
            if data:
                try:
                    compass_angle, distance = map(float, data.split(','))
                    angles.append(compass_angle)
                    distances.append(distance)
                    print(f"Compass Angle: {compass_angle}, Distance: {distance}")
                    
                    # Convert polar to Cartesian coordinates
                    radians = math.radians(compass_angle)
                    x = distance * math.cos(radians)
                    y = distance * math.sin(radians)
                    
                    # Update the plot
                    ax.clear()
                    ax.set_xlim(-MAX_DISTANCE, MAX_DISTANCE)
                    ax.set_ylim(-MAX_DISTANCE, MAX_DISTANCE)
                    for angle, distance in zip(angles, distances):
                        radians = math.radians(angle)
                        x = distance * math.cos(radians)
                        y = distance * math.sin(radians)
                        ax.scatter(x, y, c='blue')
                except ValueError:
                    pass  # Ignore lines that cannot be parsed
        except UnicodeDecodeError:
            pass  # Ignore lines that cause decoding errors

# Create a figure and axis
fig, ax = plt.subplots()
ax.set_xlim(-MAX_DISTANCE, MAX_DISTANCE)
ax.set_ylim(-MAX_DISTANCE, MAX_DISTANCE)

# Animate the plot
ani = animation.FuncAnimation(fig, update, interval=100, cache_frame_data=False)

# Show the plot
plt.show()

# Close the serial connection when done
ser.close()
