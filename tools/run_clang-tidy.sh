#!/bin/bash

#select all source files at first
files=(`find ../src/ -name *.cc`)

#auto generated files
skip=( "../src/header_scanner/lex.cc"  \
       "../src/header_parser/parse.cc" \
       "../src/log_scanner/lex.cc"     \
       "../src/log_parser/parse.cc" )

#remove auto generated files
for path in "${skip[@]}"
do
    files=(${files[@]//$path})
done

echo "Will scan: "

for path in "${files[@]}"
do
    echo "    $path"
done

echo -e "\n"

clang-tidy-15 --config-file clang-tidy-config "${files[@]}" -- -std=c++20
