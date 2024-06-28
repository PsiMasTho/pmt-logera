#!/bin/python3

# Note: for now this needs to be run from the root of the project as:
# python3 tools/run_clang-format.py

import os

directories = ["test/", "logera/", "logera_to_csv/", "logera_dump_ast/", "logera_format"]

extensions = ["cpp", "hpp"]

file_list = []

for dir in directories:
    for ext in extensions:
        for root, dirs, files in os.walk(dir):
            for file in files:
                if file.endswith("." + ext):
                    file_list.append(os.path.join(root, file))

os.system("clang-format-16 -i -style=file --verbose " + " ".join(file_list))
