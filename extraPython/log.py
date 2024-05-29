import os

def write_to_file(file_path, data):
    try:
        with open(file_path, 'a') as file:
            file.write(data + '\n')
        print(f"Appended data to {file_path}")
    except Exception as e:
        print(f"Error writing to {file_path}: {e}")

# Example usage
write_to_file('./log.txt', '85,100,100,100,100,1')
