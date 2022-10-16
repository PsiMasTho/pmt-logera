#!/bin/bash

cd ..

excluded_dir_list=('src/header_scanner' 'src/header_parser' 'src/log_parser' 'src/log_scanner')
findargs=()

for i in "${excluded_dir_list[@]}"; do
       findargs+=('!' '-path' "./$i/*")
done

file_list=()
while IFS= read -d $'\0' -r file ; do
    file_list=("${file_list[@]}" "$file")
done < <(find . -type f -name "*.cc" "${findargs[@]}" -print0)

while IFS= read -d $'\0' -r file ; do
    file_list=("${file_list[@]}" "$file")
done < <(find . -type f -name "*.h" "${findargs[@]}" -print0)

while IFS= read -d $'\0' -r file ; do
    file_list=("${file_list[@]}" "$file")
done < <(find . -type f -name "*.ih" "${findargs[@]}" -print0)

while IFS= read -d $'\0' -r file ; do
    file_list=("${file_list[@]}" "$file")
done < <(find . -type f -name "*.hi" "${findargs[@]}" -print0)

#echo "${file_list[@]}"
#echo "${findargs[@]}"

clang-format-14 -i -style=file --verbose "${file_list[@]}"
