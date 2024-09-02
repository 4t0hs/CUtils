import os
import sys


def search_string_in_files(directory: str, search_string: str) -> None:
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            try:
                with open(file_path, "r", encoding="utf-8") as f:
                    for line in f:
                        if search_string in line:
                            print(f"Found in {file_path}")
                            break
            except Exception as e:
                pass


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python search.py <directory_path> <search_string>")
        sys.exit(1)

    directory_path = sys.argv[1]
    search_string = sys.argv[2]

    search_string_in_files(directory_path, search_string)
