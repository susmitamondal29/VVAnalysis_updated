def count_string_occurrences(file_path, target_string):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
            count = content.count(target_string)
            return count
    except FileNotFoundError:
        print(f"File not found: {file_path}")
        return 0
    except Exception as e:
        print(f"An error occurred: {e}")
        return 0

# Get input from user
file_path = input("Enter the file path: ")
target_string = input("Enter the string to search for: ")

# Count occurrences
occurrences = count_string_occurrences(file_path, target_string)
print(f"The string '{target_string}' occurred {occurrences} times in the file.")

