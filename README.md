# Real-time Sensor Data Plotting and Analysis

This project involves real-time data plotting and analysis using sensor data from an Arduino setup. The project utilizes sensors such as ultrasonic sensors, a GY26 compass sensor, and a TFMini LiDAR sensor. The data is visualized through scatter plots, compass visualization, and shape analysis.

## Table of Contents
- [Features](#features)
- [Setup](#setup)
- [Wiring](#wiring)
- [Usage](#usage)
- [Files](#files)

## Features
- Real-time plotting of sensor data.
- Visualization of sensor data on scatter plots.
- Compass visualization.
- Analysis of nearest and farthest points from the origin.
- Data logging for further analysis.
- Conversion of log files to Excel format.

## Setup
1. **Hardware Requirements:**
   - Arduino with connected sensors:
     - Ultrasonic sensors
     - GY26 compass sensor
     - TFMini LiDAR sensor
   - Push button

2. **Software Requirements:**
   - Python 3.x
   - `matplotlib` for plotting
   - `numpy` for numerical computations
   - `pandas` for data processing
   - `openpyxl` for Excel file handling
   - Arduino IDE

3. **Installation:**
   - Clone the repository:
     ```bash
     git clone https://github.com/snackmasta/Lidar-Ultrasonic_Mapping.git
     ```
   - Navigate to the project directory:
     ```bash
     cd Lidar-Ultrasonic_Mapping
     ```
   - Install the required Python packages:
     ```bash
     pip install -r requirements.txt
     ```

4. **Arduino Setup:**
   - Open the Arduino IDE and upload the `main.ino` code to your Arduino board.
   - Connect the sensors to the Arduino according to the pin definitions in the code.

## Wiring
```bash
Arduino Pins:
5V  ------> VCC (TFMini LiDAR, GY26 Compass, Ultrasonic Sensors)
GND ------> GND (TFMini LiDAR, GY26 Compass, Ultrasonic Sensors, Pushbutton)

Pins:
4   ------> RX (TFMini LiDAR)
5   ------> TX (TFMini LiDAR)
6   ------> One pin of Pushbutton (Other pin to GND)
7   ------> Echo (Ultrasonic Sensor 3)
8   ------> Trigger (Ultrasonic Sensor 3)
9   ------> Echo (Ultrasonic Sensor 2)
10  ------> Trigger (Ultrasonic Sensor 2)
11  ------> Echo (Ultrasonic Sensor 1)
12  ------> Trigger (Ultrasonic Sensor 1)
A4  ------> SDA (GY26 Compass)
A5  ------> SCL (GY26 Compass)
```

## Usage
1. **Running the Scripts:**
   - Use the provided `run.bat` script to start the Python scripts for real-time plotting and log tracing:
     1. Navigate to the project directory in the command prompt.
     2. Run the batch script:
        ```bash
        run.bat
        ```
     3. Enter the COM port where the Arduino is connected when prompted.

2. **Real-time Plotting:**
   - Ensure the Arduino is connected to the computer.
   - The `run.bat` script will automatically clear previous logs, prompt for the COM port, and start the `M5-scatter-map.py` and `logTracing.py` scripts in separate command prompt windows.

3. **Converting Logs to Excel:**
   - Use the `txt2excel.py` script to convert log files to Excel format:
     ```bash
     python txt2excel.py
     ```
   - This script processes all log files in the `./Log/` directory and saves them as Excel files in the `./logTable/` directory.

## Files
- `main.ino`: Arduino code for reading sensor data and sending it via serial communication.
- `M5-scatter-map.py`: Script for real-time data plotting.
- `logTracing.py`: Script for analyzing logged data.
- `txt2excel.py`: Script for converting log files to Excel format.
- `run.bat`: Batch script to clear logs, prompt for COM port, and run Python scripts.
- `requirements.txt`: List of required Python packages.