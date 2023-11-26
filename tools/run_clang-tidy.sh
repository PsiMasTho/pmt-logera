#!/bin/bash

#select all source files at first
files=(`find ../src/ -name *.cc`)

#auto generated files and external libs
skip=( "../external/argparse/argparse.hpp" )

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

clang-tidy-17 --config-file clang-tidy-config "${files[@]}" -- -std=c++20
