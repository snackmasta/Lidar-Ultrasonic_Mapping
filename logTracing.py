import os
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
import time
import numpy as np
from collections import deque
from sklearn.linear_model import LinearRegression

# Constants
MAX_DISTANCE = 200
TIME_WINDOW = 60  # Time window for the angle-time plot

# Setup the log file connection
log_file_path = './log/log1.txt'
log_file = open(log_file_path, 'r')

# Dictionary to store latest data for each angle
data_dict = {}

# List to store all points
all_points = []

# Deques to store timestamps and angles for the angle-time plot
timestamps = deque(maxlen=TIME_WINDOW)
angles = deque(maxlen=TIME_WINDOW)

# Create a figure and axis for the scatter plot
fig, ax = plt.subplots()
fig.canvas.manager.set_window_title("Shape Analysis")
plt.title("Shape Analysis")
ax.set_xlim(-MAX_DISTANCE, MAX_DISTANCE)
ax.set_ylim(-MAX_DISTANCE, MAX_DISTANCE)
scatter1 = ax.scatter([], [], c='blue', s=10, label='frontSonar')  # Adjusted size
scatter2 = ax.scatter([], [], c='red', s=10, label='LiDAR')  # Adjusted size
scatter3 = ax.scatter([], [], c='green', s=10, label='rightSonar')  # Adjusted size
scatter4 = ax.scatter([], [], c='purple', s=10, label='leftSonar')  # Adjusted size
ax.legend()

# Create a figure and axis for the compass
# compass_fig, compass_ax = plt.subplots()
# compass_ax.set_xlim(-1.5, 1.5)
# compass_ax.set_ylim(-1.5, 1.5)
# compass_needle, = compass_ax.plot([0, 1], [0, 0], 'r-')  # Initial compass needle
# compass_ax.add_patch(plt.Circle((0, 0), 1, color='b', fill=False))  # Outer circle

# Create a figure and axis for the nearest and farthest points
# list_fig, list_ax = plt.subplots()
# list_ax.axis('off')  # Hide the axes
# nearest_text = list_ax.text(0.1, 0.9, '', fontsize=12, verticalalignment='top')
# farthest_text = list_ax.text(0.1, 0.6, '', fontsize=12, verticalalignment='top')

# Arrows for nearest and farthest points
nearest_arrow = None
farthest_arrow = None

# Text annotations for nearest and farthest points
nearest_annotation = None
farthest_annotation = None

# Add a black dot at the origin (0,0)
ax.scatter([0], [0], color='black', zorder=5)

# Create a figure and axis for the angle-time plot
# angle_time_fig, angle_time_ax = plt.subplots()
# angle_time_ax.set_ylim(0, 360)
# angle_time_plot, = angle_time_ax.plot([], [], 'b-')  # Line plot for angle over time

# Initialize start time
start_time = time.time()

# Initialize previous compass angle
previous_angle = 0

# Initialize angleDifference between angles
angle_diff = 0  # Initialize angle_diff

recent_anglesList = [] # List of angles

def log_data(log_number, data):
    """Log the data to a file with the given log number."""
    log_dir = './log'
    os.makedirs(log_dir, exist_ok=True)
    log_file = os.path.join(log_dir, f'traceLog{log_number}.txt')
    with open(log_file, 'a') as file:
        file.write(data + '\n')

def update(frame):
    global nearest_arrow, farthest_arrow, nearest_annotation, farthest_annotation, previous_angle, compass_angle, angle_diff, recent_anglesList
    try:
        data = log_file.readline().strip()
        if data:
            parts = list(map(float, data.split(',')))
            if len(parts) >= 6:
                compass_angle = parts[0]
                distance1 = parts[1]  # distance for front ultrasonic
                distance2 = parts[2]  # distance for front LiDAR
                distance3 = parts[3]  # distance for right ultrasonic
                distance4 = parts[4]  # distance for left ultrasonic
                log_number = int(parts[5])

                # Only process angles in 1-degree steps
                if compass_angle % 1!= 0:
                    return

                # Store the latest data for each angle
                data_dict[compass_angle] = (distance1, distance2, distance3, distance4)
                
                # Keep track of the 5 most recent angles in data_dict
                recent_angles = sorted(data_dict.keys(), reverse=True)[:5]
                recent_anglesReverse = sorted(data_dict.keys())[:5]
                recent_anglesList = list(data_dict.keys())[:5]
                
                # Previous angle and current angle difference
                angle_diff = abs(compass_angle - recent_anglesList[0])
                
                # Delete angles that are not in the recent_angles list
                for angle in list(data_dict.keys()):
                    if compass_angle > previous_angle:
                        if angle not in recent_angles and angle_diff < 45:
                            del data_dict[angle]
                        elif angle_diff > 45:
                            del data_dict[recent_anglesList[0]]
                            break
                    elif compass_angle < previous_angle:
                        if angle not in recent_anglesReverse and angle_diff < 45:
                            del data_dict[angle]
                        elif angle_diff > 45:
                            del data_dict[recent_anglesList[0]]
                            break
                
                previous_angle = compass_angle
                
                # Log the data
                log_data(log_number, data)

                print(f"Compass Angle: {compass_angle}, FrontSonar: {distance1}, FrontLiDAR: {distance2}, RightSonar: {distance3}, LeftSonar: {distance4}, Log Number: {log_number}")

                # Convert polar to Cartesian coordinates
                radians = -math.radians(compass_angle)
                radians += math.radians(90)
                radians_right = -math.radians(compass_angle + 90) + math.radians(90)
                radians_left = -math.radians(compass_angle - 90) + math.radians(90)
                x1 = distance1 * math.cos(radians)
                y1 = distance1 * math.sin(radians)
                x2 = distance2 * math.cos(radians)
                y2 = distance2 * math.sin(radians)
                x3 = distance3 * math.cos(radians_right)
                y3 = distance3 * math.sin(radians_right)
                x4 = distance4 * math.cos(radians_left)
                y4 = distance4 * math.sin(radians_left)

                # Add new points to the all_points list
                all_points.append((compass_angle, x1, y1))
                all_points.append((compass_angle, x2, y2))
                all_points.append((compass_angle, x3, y3))
                all_points.append((compass_angle, x4, y4))

                # Update all points on the scatter plot
                scatter1_data = []
                scatter2_data = []
                scatter3_data = []
                scatter4_data = []
                all_distances = []
                front_sonar_points = []
                right_sonar_points = []
                left_sonar_points = []

                for angle, (d1, d2, d3, d4) in data_dict.items():
                    rad = -math.radians(angle)
                    rad += math.radians(90)
                    rad_right = -math.radians(angle + 90) + math.radians(90)
                    rad_left = -math.radians(angle - 90) + math.radians(90)
                    x1_scatter = d1 * math.cos(rad)
                    y1_scatter = d1 * math.sin(rad)
                    x2_scatter = d2 * math.cos(rad)
                    y2_scatter = d2 * math.sin(rad)
                    x3_scatter = d3 * math.cos(rad_right)
                    y3_scatter = d3 * math.sin(rad_right)
                    x4_scatter = d4 * math.cos(rad_left)
                    y4_scatter = d4 * math.sin(rad_left)
                    scatter1_data.append((x1_scatter, y1_scatter))
                    scatter2_data.append((x2_scatter, y2_scatter))
                    scatter3_data.append((x3_scatter, y3_scatter))
                    scatter4_data.append((x4_scatter, y4_scatter))

                    all_distances.append((angle, d1, (x1_scatter, y1_scatter)))
                    all_distances.append((angle, d2, (x2_scatter, y2_scatter)))
                    all_distances.append((angle, d3, (x3_scatter, y3_scatter)))
                    all_distances.append((angle, d4, (x4_scatter, y4_scatter)))
                    
                    # Collect sonar points for linear regression
                    front_sonar_points.append((x1_scatter, y1_scatter))
                    right_sonar_points.append((x3_scatter, y3_scatter))
                    left_sonar_points.append((x4_scatter, y4_scatter))

                scatter1.set_offsets(np.array(scatter1_data))
                scatter2.set_offsets(np.array(scatter2_data))
                scatter3.set_offsets(np.array(scatter3_data))
                scatter4.set_offsets(np.array(scatter4_data))

                # Update the compass needle
                # compass_needle.set_data([0, math.cos(radians)], [0, math.sin(radians)])

                # Find nearest and farthest points from the origin
                nearest_point = min(all_distances, key=lambda item: math.sqrt(item[2][0]**2 + item[2][1]**2))
                farthest_point = max(all_distances, key=lambda item: math.sqrt(item[2][0]**2 + item[2][1]**2))

                # nearest_text.set_text(f'Nearest Point:\nAngle: {nearest_point[0]}\nDistance: {nearest_point[1]:.2f}')
                # farthest_text.set_text(f'Farthest Point:\nAngle: {farthest_point[0]}\nDistance: {farthest_point[1]:.2f}')

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

                # Linear Regression for front sonar data
                if len(front_sonar_points) > 1:
                    X = np.array(front_sonar_points)[:, 0].reshape(-1, 1)
                    y = np.array(front_sonar_points)[:, 1]
                    model = LinearRegression().fit(X, y)
                    x_range = np.linspace(-MAX_DISTANCE, MAX_DISTANCE, 100)
                    y_range = model.predict(x_range.reshape(-1, 1))
                    if hasattr(update, 'lr_line_front'):
                        update.lr_line_front.remove()
                    update.lr_line_front, = ax.plot(x_range, y_range, color='cyan', linewidth=2, label='Front Sonar Regression')
                    print(f"Front Sonar Regression Line: y = {model.coef_[0]:.2f}x + {model.intercept_:.2f}")

                # Perpendicular Regression for right sonar data
                if len(right_sonar_points) > 1:
                    X = np.array(right_sonar_points)[:, 1].reshape(-1, 1)
                    y = np.array(right_sonar_points)[:, 0]
                    model = LinearRegression().fit(X, y)
                    y_range = np.linspace(-MAX_DISTANCE, MAX_DISTANCE, 100)
                    x_range = model.predict(y_range.reshape(-1, 1))
                    if hasattr(update, 'lr_line_right'):
                        update.lr_line_right.remove()
                    update.lr_line_right, = ax.plot(x_range, y_range, color='magenta', linewidth=2, label='Right Sonar Regression')
                    print(f"Right Sonar Regression Line: x = {model.coef_[0]:.2f}y + {model.intercept_:.2f}")

                # Perpendicular Regression for left sonar data
                if len(left_sonar_points) > 1:
                    X = np.array(left_sonar_points)[:, 1].reshape(-1, 1)
                    y = np.array(left_sonar_points)[:, 0]
                    model = LinearRegression().fit(X, y)
                    y_range = np.linspace(-MAX_DISTANCE, MAX_DISTANCE, 100)
                    x_range = model.predict(y_range.reshape(-1, 1))
                    if hasattr(update, 'lr_line_left'):
                        update.lr_line_left.remove()
                    update.lr_line_left, = ax.plot(x_range, y_range, color='purple', linewidth=2, label='Left Sonar Regression')
                    print(f"Left Sonar Regression Line: x = {model.coef_[0]:.2f}y + {model.intercept_:.2f}")

                # Update the angle-time plot
                current_time = time.time() - start_time
                timestamps.append(current_time)
                angles.append(compass_angle)

                # Convert timestamps to relative time (seconds)
                # angle_time_plot.set_data(timestamps, angles)

                # Adjust x-axis limits dynamically to current time
                # if len(timestamps) > 1:
                #     angle_time_ax.set_xlim(max(0, timestamps[0]), current_time)

    except (UnicodeDecodeError, ValueError) as e:
        print(f"Error processing data: {e}")  # Print error message

    # Redraw all figures
    fig.canvas.draw_idle()
    # compass_fig.canvas.draw_idle()
    # list_fig.canvas.draw_idle()
    # angle_time_fig.canvas.draw_idle()

# Animate all plots
ani1 = animation.FuncAnimation(fig, update, interval=10, cache_frame_data=False)
# ani2 = animation.FuncAnimation(compass_fig, update, interval=10, cache_frame_data=False)
# ani3 = animation.FuncAnimation(list_fig, update, interval=10, cache_frame_data=False)
# ani4 = animation.FuncAnimation(angle_time_fig, update, interval=10, cache_frame_data=False)

# Show the plots
plt.show()

# Close the log file when done
log_file.close()
