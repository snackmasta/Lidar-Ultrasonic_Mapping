import os
import pandas as pd

# Directory containing the log files
log_dir = './Log/'

# Get a list of all log files in the directory
log_files = [f for f in os.listdir(log_dir) if f.startswith('log') and f.endswith('.txt')]

# Process each log file
for log_file in log_files:
    # Read the log file
    with open(os.path.join(log_dir, log_file), 'r') as file:
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

    # Determine the output file name
    log_number = log_file[3:-4]  # Extract log number from file name
    output_file = f'./logTable/logTable{log_number}.xlsx'

    # Write the DataFrame to an Excel file
    df_transposed.to_excel(output_file, header=False, index=False)

    print(f"Data has been written to {output_file}")
