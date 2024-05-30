@echo off

:: Specify the path to the text file
set FILE_PATH=./Log/log1.txt

:: Clear the content of the file by redirecting an empty string to it
echo. > %FILE_PATH%
del "./COM.txt"

:: Specify the path to the text file
set COM_PATH=./COM.txt

:: Prompt the user for the COM port
set /p COM_PORT="Enter the COM port: "

:: Append the COM port to the text file
echo COM%COM_PORT% >> %COM_PATH%

:: Open a new command prompt window
start cmd

:: Start the M5 Scatter Plot script in a separate command prompt window
start cmd /k "python3 M5-scatter-map.py"

:: Wait for 5 seconds to allow the M5 Scatter Plot script to start
timeout /t 5 > NUL

:: Start the Log Tracing script in the same command prompt window
python3 logTracing.py

:: Wait for the scripts to finish
pause