import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
import time
import numpy as np

# Constants
MAX_DISTANCE = 200
MAX_TRACE_DISTANCE = 50

# Setup the serial port connection
ser = serial.Serial('COM21', 9600, timeout=1)
time.sleep(5)  # Allow time for the connection to establish

# Dictionary to store latest data for each angle
data_dict = {}

# Create a figure and axis for the scatter plot
fig, ax = plt.subplots()
ax.set_xlim(-MAX_DISTANCE, MAX_DISTANCE)
ax.set_ylim(-MAX_DISTANCE, MAX_DISTANCE)
scatter1 = ax.scatter([], [], c='blue', label='Ultrasonic')
scatter2 = ax.scatter([], [], c='red', label='Lidar')
ax.legend()

# Create a figure and axis for the compass
compass_fig, compass_ax = plt.subplots()
compass_ax.set_xlim(-1.5, 1.5)
compass_ax.set_ylim(-1.5, 1.5)
compass_needle, = compass_ax.plot([0, 1], [0, 0], 'r-')  # Initial compass needle
compass_ax.add_patch(plt.Circle((0, 0), 1, color='b', fill=False))  # Outer circle

# Create a figure and axis for the nearest and farthest points
list_fig, list_ax = plt.subplots()
list_ax.axis('off')  # Hide the axes
nearest_text = list_ax.text(0.1, 0.9, '', fontsize=12, verticalalignment='top')
farthest_text = list_ax.text(0.1, 0.6, '', fontsize=12, verticalalignment='top')

# Maintain previous coordinates for tracing lines
prev_x1 = None
prev_y1 = None
prev_x2 = None
prev_y2 = None

# Initialize annotations for nearest and farthest points
nearest_annotation = ax.annotate('', xy=(0, 0), xytext=(0, 0),
                                 arrowprops=dict(facecolor='green', shrink=0.05))
farthest_annotation = ax.annotate('', xy=(0, 0), xytext=(0, 0),
                                  arrowprops=dict(facecolor='purple', shrink=0.05))

def update(frame):
    global prev_x1, prev_y1, prev_x2, prev_y2
    if ser.in_waiting > 0:
        try:
            data = ser.readline().decode('utf-8', errors='ignore').strip()
            if data:
                parts = list(map(float, data.split(',')))
                if len(parts) >= 3:
                    compass_angle = parts[0]
                    distance1 = parts[1]
                    distance2 = parts[2]
                    
                    # Store the latest data for each angle
                    data_dict[compass_angle] = (distance1, distance2)
                    
                    print(f"Compass Angle: {compass_angle}, Distance 1: {distance1}, Distance 2: {distance2}")

                    # Convert polar to Cartesian coordinates
                    radians = math.radians(compass_angle)
                    x1 = distance1 * math.cos(radians)
                    y1 = distance1 * math.sin(radians)
                    x2 = distance2 * math.cos(radians)
                    y2 = distance2 * math.sin(radians)

                    # Update all points on the scatter plot
                    scatter1_data = []
                    scatter2_data = []
                    all_distances = []
                    for angle, (d1, d2) in data_dict.items():
                        rad = math.radians(angle)
                        x1_scatter = d1 * math.cos(rad)
                        y1_scatter = d1 * math.sin(rad)
                        x2_scatter = d2 * math.cos(rad)
                        y2_scatter = d2 * math.sin(rad)
                        scatter1_data.append((x1_scatter, y1_scatter))
                        scatter2_data.append((x2_scatter, y2_scatter))
                        all_distances.append((angle, d1, (x1_scatter, y1_scatter)))
                        all_distances.append((angle, d2, (x2_scatter, y2_scatter)))
                    
                    scatter1.set_offsets(np.array(scatter1_data))
                    scatter2.set_offsets(np.array(scatter2_data))

                    # Draw lines connecting to the nearest previous point if within max trace distance
                    if prev_x1 is not None and prev_y1 is not None:
                        if math.sqrt((x1 - prev_x1)**2 + (y1 - prev_y1)**2) <= MAX_TRACE_DISTANCE:
                            ax.plot([prev_x1, x1], [prev_y1, y1], 'green')
                    if prev_x2 is not None and prev_y2 is not None:
                        if math.sqrt((x2 - prev_x2)**2 + (y2 - prev_y2)**2) <= MAX_TRACE_DISTANCE:
                            ax.plot([prev_x2, x2], [prev_y2, y2], 'green')

                    # Update previous coordinates
                    prev_x1, prev_y1 = x1, y1
                    prev_x2, prev_y2 = x2, y2

                    # Update the compass needle
                    compass_needle.set_data([0, math.cos(radians)], [0, math.sin(radians)])

                    # Find nearest and farthest points from the origin
                    nearest_point = min(all_distances, key=lambda item: math.sqrt(item[2][0]**2 + item[2][1]**2))
                    farthest_point = max(all_distances, key=lambda item: math.sqrt(item[2][0]**2 + item[2][1]**2))

                    nearest_text.set_text(f'Nearest Point:\nAngle: {nearest_point[0]}\nDistance: {nearest_point[1]:.2f}')
                    farthest_text.set_text(f'Farthest Point:\nAngle: {farthest_point[0]}\nDistance: {farthest_point[1]:.2f}')

                    # Update annotations for nearest and farthest points
                    nearest_annotation.xy = nearest_point[2]
                    nearest_annotation.set_text(f'Angle: {nearest_point[0]}, Distance: {nearest_point[1]:.2f}')
                    nearest_annotation.set_position((nearest_point[2][0] + 5, nearest_point[2][1] + 5))

                    farthest_annotation.xy = farthest_point[2]
                    farthest_annotation.set_text(f'Angle: {farthest_point[0]}, Distance: {farthest_point[1]:.2f}')
                    farthest_annotation.set_position((farthest_point[2][0] + 5, farthest_point[2][1] + 5))

        except UnicodeDecodeError:
            pass  # Ignore lines that cause decoding errors

    # Redraw all figures
    fig.canvas.draw_idle()
    compass_fig.canvas.draw_idle()
    list_fig.canvas.draw_idle()

# Animate all plots
ani1 = animation.FuncAnimation(fig, update, interval=10, cache_frame_data=False)
ani2 = animation.FuncAnimation(compass_fig, update, interval=10, cache_frame_data=False)
ani3 = animation.FuncAnimation(list_fig, update, interval=10, cache_frame_data=False)

# Show the plots
plt.show()

# Close the serial connection when done
ser.close()
