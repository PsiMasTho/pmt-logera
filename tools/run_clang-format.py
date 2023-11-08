#!/bin/python3

import os

directories = ["test/", "include/", "src/"]

extensions = ["cc", "h", "ih", "inl"]

    # Exclude auto-generated files from bisonc++ and flexc++
exclude_files = ["header_scanner_base.h", "log_parser.ih", "log_scanner_lex.cc", "header_scanner.h", "log_parser_parse.cc", "header_parserbase.h",
                 "header_parser.h", "header_scanner.ih", "log_scanner_base.h", "header_parser.ih", "log_parserbase.h", "log_scanner.h", "header_parser_parse.cc", "header_scanner_lex.cc", "log_parser.h", "log_scanner.ih"]

file_list = []

for dir in directories:
    for ext in extensions:
        for root, dirs, files in os.walk(dir):
            for file in files:
                if file.endswith("." + ext) and file not in exclude_files:
                    file_list.append(os.path.join(root, file))

os.system("clang-format-15 -i -style=file --verbose " + " ".join(file_list))

