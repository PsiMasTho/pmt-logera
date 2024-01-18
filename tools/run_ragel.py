#!/bin/python3

import argparse
import os

def run_ragel(input_file, output_file):
	ragel_args = ["-G2",
				  f"-o {output_file}",
				  f"{input_file}"]
	
	ragel_args = " ".join(ragel_args)
	return os.system("ragel " + ragel_args)

def prepend_to_file(file_path, string_to_prepend):
    # Open the file in read mode and read its contents
    with open(file_path, 'r') as file:
        original_contents = file.read()

    # Open the file in write mode, write the string to prepend, and then write the original contents
    with open(file_path, 'w') as file:
        file.write(string_to_prepend + '\n' + original_contents)

def main(args):
	# Run ragel
	if run_ragel(args.input_file, args.output_file) != 0:
		print("[run_ragel.py] Error running ragel.")
		exit(1)
	else:
		print("[run_ragel.py] Successfully ran ragel.")

	# Prepend autogenerated warning
	prepend_to_file(args.output_file, "// This file is autogenerated by ragel. Do not modify it manually!")
	
	print("[run_ragel.py] Done.")
	

def validate_args(args):
	if args.input_file == None:
		print("[run_ragel.py] Error: input file not specified.")
		exit(1)
	elif args.output_file == None:
		print("[run_ragel.py] Error: output file not specified.")
		exit(1)
	elif not os.path.isfile(args.input_file):
		print("[run_ragel.py] Error: input path is not a file.")
		exit(1)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Generate ragel files.")
	parser.add_argument("--input-file", help="Path to the input file.")
	parser.add_argument("--output-file", help="Path to the output file.")

	# Check if the root directory is valid.
	args = parser.parse_args()
	validate_args(args)
	main(args)