#!/bin/bash

cppcheck ../src/ --enable=all --std=c++20 --suppress=missingIncludeSystem \
    -i ../src/header_scanner/lex.cc                                       \
    -i ../src/header_parser/parse.cc                                      \
    -i ../src/log_scanner/lex.cc                                          \
    -i ../src/log_parser/parse.cc
