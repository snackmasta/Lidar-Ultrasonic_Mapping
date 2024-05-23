import pandas as pd

# Read the log file
with open('./Log/log1.txt', 'r') as file:
    lines = file.readlines()

# Initialize lists to store data
angles = []
ultrasonic_data = []
lidar_data = []

# Process each line
for line in lines:
    if line.strip():  # Ensure the line is not empty
        parts = line.split(',')
        if len(parts) == 4:
            angle, ultrasonic, lidar, log_number = parts
            angles.append(int(angle))
            ultrasonic_data.append(int(ultrasonic))
            lidar_data.append(int(lidar))

# Create a DataFrame
df = pd.DataFrame({
    'Angle': angles,
    'Ultrasonic': ultrasonic_data,
    'LiDAR': lidar_data
})

# Transpose the DataFrame so that data is in the required format
df_transposed = df.T

# Insert headers as the first column
headers = ['Angle', 'Ultrasonic', 'LiDAR']
df_transposed.insert(0, 'Header', headers)

# Write the DataFrame to an Excel file
df_transposed.to_excel('output1.xlsx', header=False, index=False)

print("Data has been written to output.xlsx")
