#!/bin/python3

import argparse
import os
import tempfile

def replace_file_extension(file_name, new_extension):
	# Remove the old extension
	file_name = file_name.split(".")[0]
	return file_name + "." + new_extension

def run_ragel(input_file, output_file):
	ragel_args = ["-G2",
				  "-L",
					f"-o {output_file}",
					f"{input_file}"]
	
	ragel_args = " ".join(ragel_args)
	return os.system("ragel " + ragel_args)

def main(args):
	input_dir = args.root_dir + "/src/grammar_lex/"
	output_dir = args.root_dir + "/src/lexer/"

	# Run ragel for header grammar first
	if run_ragel(input_dir + "header_lexer.rl", output_dir + "/header_lexer.cc") != 0:
		print("Error running ragel for header.")
		exit(1)
	else:
		print("Successfully ran ragel for header.")
	
	# Run ragel for log grammar
	if run_ragel(input_dir + "log_lexer.rl", output_dir + "/log_lexer.cc") != 0:
		print("Error running ragel for log.")
		exit(1)
	else:
		print("Successfully ran ragel for log.")
	
	print("Done.")
	

def validate_args(args):
	if args.root_dir is None:
		print("Root directory is not specified.")
		exit(1)
	if not os.path.isdir(args.root_dir):
		print("Root directory is not a directory.")
		exit(1)
	if not os.path.isdir(args.root_dir + "/src/"):
		print("Root directory does not contain a src directory.")
		exit(1)

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Generate ragel files.")
	parser.add_argument("--root-dir", help="Root directory of the project.")

	# Check if the root directory is valid.
	args = parser.parse_args()
	validate_args(args)
	main(args)