import numpy as np
import matplotlib.pyplot as plt

# Define the coefficients for the regression lines
# Front Sonar Regression Line: y = -0.54x + 104.58
m_front, b_front = -0.54, 104.58
# Right Sonar Regression Line: x = -0.00y + -49.87 -> y = -49.87 / -0.00
m_right, b_right = 0, -49.87  # Note: m_right is effectively zero, we need to handle this case
# Left Sonar Regression Line: x = -0.00y + 99.75 -> y = 99.75 / -0.00
m_left, b_left = 0, 99.75  # Note: m_left is effectively zero, we need to handle this case

# Create a range of x values
x = np.linspace(-200, 200, 400)
# Create a range of y values for lines where m = 0
y = np.linspace(-200, 200, 400)

# Calculate y values for the Front Sonar line
y_front = m_front * x + b_front

# Since m_right and m_left are effectively zero, the lines are vertical
# Right Sonar: x = -49.87
x_right = np.full_like(y, b_right)
# Left Sonar: x = 99.75
x_left = np.full_like(y, b_left)

# Plot the lines
plt.plot(x, y_front, label='Front Sonar Regression Line: y = -0.54x + 104.58')
plt.axvline(x=b_right, color='orange', linestyle='--', label='Right Sonar Regression Line: x = -49.87')
plt.axvline(x=b_left, color='green', linestyle='--', label='Left Sonar Regression Line: x = 99.75')

# Find the intersection points manually since two lines are vertical
# Intersection of Front Sonar and Right Sonar
intersection_x_right = b_right
intersection_y_right = m_front * intersection_x_right + b_front

# Intersection of Front Sonar and Left Sonar
intersection_x_left = b_left
intersection_y_left = m_front * intersection_x_left + b_front

# Plot the intersection points
plt.scatter(intersection_x_right, intersection_y_right, color='red')
plt.text(intersection_x_right, intersection_y_right, f'  ({intersection_x_right:.2f}, {intersection_y_right:.2f})', color='red')
plt.scatter(intersection_x_left, intersection_y_left, color='blue')
plt.text(intersection_x_left, intersection_y_left, f'  ({intersection_x_left:.2f}, {intersection_y_left:.2f})', color='blue')

# Add labels and a legend
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.title('Intersection of Sonar Regression Lines')
plt.grid(True)

# Show the plot
plt.show()
