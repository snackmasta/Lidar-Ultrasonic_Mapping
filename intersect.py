import numpy as np
import matplotlib.pyplot as plt
from sympy import symbols, Eq, solve

# Define parameters for the lines
slope = 0
intercept = 130.15

# Define the line equations
def line1(x):
    return slope * x + intercept

def line2(y):
    return (-slope * y - intercept)

def line3(y):
    return (-slope * y + intercept)

# Generate x values for plotting
x_values = np.linspace(-200, 200, 400)  # Adjust the range as needed

# Plot the lines
plt.figure(figsize=(8, 6))

plt.plot(x_values, line1(x_values), label=f'y = {slope}x + {intercept}')
plt.plot(line2(x_values), x_values, label=f'x = {-slope}y - {intercept}')
plt.plot(line3(x_values), x_values, label=f'x = {-slope}y + {intercept}')

# Find intersection points
x, y = symbols('x y')
eq1 = Eq(y, slope * x + intercept)
eq2 = Eq(x, -slope * y - intercept)
eq3 = Eq(x, -slope * y + intercept)

solution1 = solve((eq1, eq2), (x, y))
solution2 = solve((eq1, eq3), (x, y))

intersection_x1, intersection_y1 = solution1[x], solution1[y]
intersection_x2, intersection_y2 = solution2[x], solution2[y]

plt.plot(intersection_x1, intersection_y1, 'ro', label=f'Intersection 1 ({intersection_x1:.2f}, {intersection_y1:.2f})')
plt.plot(intersection_x2, intersection_y2, 'go', label=f'Intersection 2 ({intersection_x2:.2f}, {intersection_y2:.2f})')

plt.xlabel('x')
plt.ylabel('y')
plt.title('Plot of lines and their intersections')
plt.legend()
plt.grid(True)
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.show()

print(f"Intersection 1: ({intersection_x1:.2f}, {intersection_y1:.2f})")
print(f"Intersection 2: ({intersection_x2:.2f}, {intersection_y2:.2f})")
