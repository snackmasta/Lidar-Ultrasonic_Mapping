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
distances1 = []
distances2 = []

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
                    
                    # Update the plot
                    ax.clear()
                    ax.set_xlim(-MAX_DISTANCE, MAX_DISTANCE)
                    ax.set_ylim(-MAX_DISTANCE, MAX_DISTANCE)
                    ax.scatter(x1, y1, c='blue', label='Distance 1')
                    ax.scatter(x2, y2, c='red', label='Distance 2')
                    ax.legend()
                    
                    # Plot all previously recorded points
                    for i in range(len(angles)):
                        rad = math.radians(angles[i])
                        xx1 = distances1[i] * math.cos(rad)
                        yy1 = distances1[i] * math.sin(rad)
                        xx2 = distances2[i] * math.cos(rad)
                        yy2 = distances2[i] * math.sin(rad)
                        ax.scatter(xx1, yy1, c='blue', alpha=0.5)
                        ax.scatter(xx2, yy2, c='red', alpha=0.5)
                    
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
