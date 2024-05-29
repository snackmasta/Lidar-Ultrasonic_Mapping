import os
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
import time
import numpy as np
from collections import deque

# Constants
MAX_DISTANCE = 200
TIME_WINDOW = 60  # Time window for the angle-time plot

# Setup the serial port connection
ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(5)  # Allow time for the connection to establish

# Dictionary to store latest data for each angle
data_dict = {}

# List to store all points
all_points = []

# Deques to store timestamps and angles for the angle-time plot
timestamps = deque(maxlen=TIME_WINDOW)
angles = deque(maxlen=TIME_WINDOW)

# Create a figure and axis for the scatter plot
fig, ax = plt.subplots()
ax.set_xlim(-MAX_DISTANCE, MAX_DISTANCE)
ax.set_ylim(-MAX_DISTANCE, MAX_DISTANCE)
scatter1 = ax.scatter([], [], c='blue', s=10, label='Sonar Kalman')  # Adjusted size
scatter2 = ax.scatter([], [], c='red', s=10, label='Sonar')  # Adjusted size
ax.legend()

# Create a figure and axis for the compass
compass_fig, compass_ax = plt.subplots()
compass_ax.set_xlim(-1.5, 1.5)
compass_ax.set_ylim(-1.5, 1.5)
compass_needle, = compass_ax.plot([0, 1], [0, 0], 'r-')  # Initial compass needle
angle_filter_needle, = compass_ax.plot([0, 1], [0, 0], 'g-')  # Additional compass needle
compass_ax.add_patch(plt.Circle((0, 0), 1, color='b', fill=False))  # Outer circle

# Create a figure and axis for the nearest and farthest points
list_fig, list_ax = plt.subplots()
list_ax.axis('off')  # Hide the axes
nearest_text = list_ax.text(0.1, 0.9, '', fontsize=12, verticalalignment='top')
farthest_text = list_ax.text(0.1, 0.6, '', fontsize=12, verticalalignment='top')

# Arrows for nearest and farthest points
nearest_arrow = None
farthest_arrow = None

# Text annotations for nearest and farthest points
nearest_annotation = None
farthest_annotation = None

# Add a black dot at the origin (0,0)
ax.scatter([0], [0], color='black', zorder=5)

# Create a figure and axis for the angle-time plot
angle_time_fig, angle_time_ax = plt.subplots()
angle_time_ax.set_ylim(0, 360)
angle_time_plot, = angle_time_ax.plot([], [], 'b-')  # Line plot for angle over time

# Initialize start time
start_time = time.time()

def log_data(log_number, data):
    """Log the data to a file with the given log number."""
    log_dir = './log'
    os.makedirs(log_dir, exist_ok=True)
    log_file = os.path.join(log_dir, f'log{log_number}.txt')
    with open(log_file, 'a') as file:
        file.write(data + '\n')

def update(frame):
    global nearest_arrow, farthest_arrow, nearest_annotation, farthest_annotation
    if ser.in_waiting > 0:
        try:
            data = ser.readline().decode('utf-8', errors='ignore').strip()
            if data:
                parts = list(map(float, data.split(',')))
                if len(parts) >= 5:
                    compass_angle = parts[0]
                    distance1 = parts[1]
                    distance2 = parts[2]
                    log_number = int(parts[3])
                    angle_filter = parts[4]

                    # Only process angles in 10-degree steps
                    if compass_angle % 1 != 0:
                        return
                    
                    # Store the latest data for each angle
                    data_dict[compass_angle] = (distance1, distance2)
                    
                    # Log the data
                    log_data(log_number, data)
                    
                    print(f"Compass Angle: {compass_angle}, Distance 1: {distance1}, Distance 2: {distance2}, Log Number: {log_number}, Angle Filter: {angle_filter}")

                    # Convert polar to Cartesian coordinates
                    radians = math.radians(compass_angle)
                    x1 = distance1 * math.cos(radians)
                    y1 = distance1 * math.sin(radians)
                    x2 = distance2 * math.cos(radians)
                    y2 = distance2 * math.sin(radians)

                    # Add new points to the all_points list
                    all_points.append((compass_angle, x1, y1))
                    all_points.append((compass_angle, x2, y2))

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

                    # Update the compass needles
                    compass_needle.set_data([0, math.cos(radians)], [0, math.sin(radians)])
                    angle_filter_radians = math.radians(angle_filter)
                    angle_filter_needle.set_data([0, math.cos(angle_filter_radians)], [0, math.sin(angle_filter_radians)])

                    # Find nearest and farthest points from the origin
                    nearest_point = min(all_distances, key=lambda item: math.sqrt(item[2][0]**2 + item[2][1]**2))
                    farthest_point = max(all_distances, key=lambda item: math.sqrt(item[2][0]**2 + item[2][1]**2))

                    nearest_text.set_text(f'Nearest Point:\nAngle: {nearest_point[0]}\nDistance: {nearest_point[1]:.2f}')
                    farthest_text.set_text(f'Farthest Point:\nAngle: {farthest_point[0]}\nDistance: {farthest_point[1]:.2f}')

                    # Add arrows for nearest and farthest points
                    if nearest_arrow:
                        nearest_arrow.remove()
                    if farthest_arrow:
                        farthest_arrow.remove()
                    nearest_arrow = ax.annotate('', xy=nearest_point[2], xytext=(0, 0),
                                                arrowprops=dict(facecolor='green', arrowstyle='->'))
                    farthest_arrow = ax.annotate('', xy=farthest_point[2], xytext=(0, 0),
                                                 arrowprops=dict(facecolor='red', arrowstyle='->'))

                    # Add Min and Max text annotations
                    if nearest_annotation:
                        nearest_annotation.remove()
                    if farthest_annotation:
                        farthest_annotation.remove()
                    nearest_annotation = ax.text(nearest_point[2][0], nearest_point[2][1], 'Min', color='green', fontsize=12)
                    farthest_annotation = ax.text(farthest_point[2][0], farthest_point[2][1], 'Max', color='red', fontsize=12)

                    # Update the angle-time plot
                    current_time = time.time() - start_time
                    timestamps.append(current_time)
                    angles.append(compass_angle)

                    # Convert timestamps to relative time (seconds)
                    angle_time_plot.set_data(timestamps, angles)

                    # Adjust x-axis limits dynamically to current time
                    if len(timestamps) > 1:
                        angle_time_ax.set_xlim(max(0, timestamps[0]), current_time)

        except (UnicodeDecodeError, ValueError) as e:
            print(f"Error processing data: {e}")  # Print error message

    # Redraw all figures
    fig.canvas.draw_idle()
    compass_fig.canvas.draw_idle()
    list_fig.canvas.draw_idle()
    angle_time_fig.canvas.draw_idle()

# Animate all plots
ani1 = animation.FuncAnimation(fig, update, interval=10, cache_frame_data=False)
ani2 = animation.FuncAnimation(compass_fig, update, interval=10, cache_frame_data=False)
ani3 = animation.FuncAnimation(list_fig, update, interval=10, cache_frame_data=False)
ani4 = animation.FuncAnimation(angle_time_fig, update, interval=10, cache_frame_data=False)

# Show the plots
plt.show()

# Close the serial connection when done
ser.close()
