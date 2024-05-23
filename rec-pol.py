import math
import serial
import time

def rectangular_to_polar(x, y):
    r = math.sqrt(x**2 + y**2)
    theta = math.atan2(y, x)
    theta_deg = math.degrees(theta)
    return theta_deg, r, r

def generate_points(corner1, corner2, step):
    x1, y1 = corner1
    x2, y2 = corner2
    points = []
    distance = math.sqrt((x2 - x1)**2 + (y2 - y1)**2)
    num_steps = int(distance // step)
    
    for i in range(num_steps + 1):
        x = x1 + (x2 - x1) * i / num_steps
        y = y1 + (y2 - y1) * i / num_steps
        points.append((x, y))
        
    return points

# Define the corners of the square (with a 50-unit left offset and 75-unit bottom offset)
square_side = 150
offset_x = -75  # 50 units to the left
offset_y = 0  # 75 units to the bottom
corners = [
    (offset_x, offset_y),
    (square_side + offset_x, offset_y),
    (square_side + offset_x, square_side + offset_y),
    (offset_x, square_side + offset_y),
    (offset_x, offset_y)  # Close the loop back to the first corner
]

# Open serial port
try:
    ser = serial.Serial('COM20')  # Adjust 'COM20' to the appropriate COM port
    time.sleep(2)  # Wait for the serial connection to initialize

    step_size = 10  # 5 units per step
    for i in range(len(corners) - 1):
        corner1 = corners[i]
        corner2 = corners[i + 1]
        points = generate_points(corner1, corner2, step_size)
        
        for point in points:
            x, y = point
            angle, distance1, distance2 = rectangular_to_polar(x, y)
            output_string = f"{angle},{distance1},{distance2}"
            ser.write((output_string + '\n').encode())  # Send each point with a newline character
            print(f"Sent to COM20: {output_string}")
            time.sleep(0.1)  # Small delay between sending points

    ser.close()
except serial.SerialException as e:
    print(f"Serial port error: {e}")
except Exception as e:
    print(f"Error: {e}")
