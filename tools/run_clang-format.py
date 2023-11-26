#!/bin/python3

# Note: for now this needs to be run from the root of the project as:
# python3 tools/run_clang-format.py

import os

directories = ["test/", "include/", "src/"]

extensions = ["cc", "h", "ih"]

file_list = []

for dir in directories:
    for ext in extensions:
        for root, dirs, files in os.walk(dir):
            for file in files:
                if file.endswith("." + ext):
                    file_list.append(os.path.join(root, file))

os.system("clang-format-17 -i -style=file --verbose " + " ".join(file_list))

