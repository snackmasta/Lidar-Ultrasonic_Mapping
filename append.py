import time

def read_and_append_lines(input_file_path, output_file_path):
    try:
        with open(input_file_path, 'r') as input_file:
            for line in input_file:
                line = line.strip()  # Strip whitespace and newlines
                if line:  # Check if line is not empty
                    print(line)  # Print each line without the trailing newline character
                    
                    # Open the output file, append the line, and close the file
                    with open(output_file_path, 'a') as output_file:
                        output_file.write(line + '\n')  # Add newline character after each line
                
                time.sleep(1)  # Wait for 1 second before reading the next line
    except FileNotFoundError:
        print(f"The file {input_file_path} does not exist.")
    except Exception as e:
        print(f"An error occurred: {e}")

input_file_path = './log/log1.txt'
output_file_path = './log/trace.txt'
read_and_append_lines(input_file_path, output_file_path)
