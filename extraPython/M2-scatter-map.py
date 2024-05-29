import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
import time
import numpy as np

# Constants
MAX_DISTANCE = 200

# Setup the serial port connection
ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(5)  # Allow time for the connection to establish

angles = []
distances1 = []
distances2 = []

# Create a figure and axis
fig, ax = plt.subplots()
ax.set_xlim(-MAX_DISTANCE, MAX_DISTANCE)
ax.set_ylim(-MAX_DISTANCE, MAX_DISTANCE)
scatter1 = ax.scatter([], [], c='blue', label='Ultrasonic')
scatter2 = ax.scatter([], [], c='red', label='Lidar')

# Function to update the plot
def update(frame):
    if ser.in_waiting > 0:
        try:
            data = ser.readline().decode('utf-8', errors='ignore').strip()
            if data:
                parts = list(map(float, data.split(',')))
                if len(parts) >= 3:
                    compass_angle = parts[0]
                    distance1 = parts[1]
                    distance2 = parts[2]
                    angles.append(compass_angle)
                    distances1.append(distance1)
                    distances2.append(distance2)
                    print(f"Compass Angle: {compass_angle}, Distance 1: {distance1}, Distance 2: {distance2}")

                    # Convert polar to Cartesian coordinates
                    radians = math.radians(compass_angle)
                    x1 = distance1 * math.cos(radians)
                    y1 = distance1 * math.sin(radians)
                    x2 = distance2 * math.cos(radians)
                    y2 = distance2 * math.sin(radians)

                    # Update the scatter plot data
                    scatter1.set_offsets(np.c_[x1, y1])
                    scatter2.set_offsets(np.c_[x2, y2])

                    # Update all previously recorded points
                    x1_all = [distances1[i] * math.cos(math.radians(angles[i])) for i in range(len(angles))]
                    y1_all = [distances1[i] * math.sin(math.radians(angles[i])) for i in range(len(angles))]
                    x2_all = [distances2[i] * math.cos(math.radians(angles[i])) for i in range(len(angles))]
                    y2_all = [distances2[i] * math.sin(math.radians(angles[i])) for i in range(len(angles))]
                    scatter1.set_offsets(np.c_[x1_all, y1_all])
                    scatter2.set_offsets(np.c_[x2_all, y2_all])

                    plt.pause(0.05)

        except UnicodeDecodeError:
            pass  # Ignore lines that cause decoding errors

# Animate the plot
ani = animation.FuncAnimation(fig, update, interval=10, cache_frame_data=False)

# Show the plot
plt.show()

# Close the serial connection when done
ser.close()
