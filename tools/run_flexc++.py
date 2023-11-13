#!/bin/python3

# cd "../src/" &&\
# flexc++ log_scanner_lexer    --no-lines &&\
# flexc++ header_scanner_lexer --no-lines &&\
# echo SUCCESS

import argparse
import os
import tempfile
import glob

def replace_str_in_file(file, old, new):
	with open(file, "r") as f:
		filedata = f.read()
	filedata = filedata.replace(old, new)
	with open(file, "w") as f:
		f.write(filedata)

def replace_str_in_files(file_list, old, new):
	for file in file_list:
		replace_str_in_file(file, old, new)

def run_flexcpp(output_dir):
	flexcpp_args = ["../src/grammar_lex/lexer",
					"--no-lines",
					f"--target-directory={output_dir}"]
	
	flexcpp_args = " ".join(flexcpp_args)
	return os.system("flexc++ " + flexcpp_args)

def main(args):
	temp_dir = tempfile.TemporaryDirectory(dir=args.root_dir)
	print("Temporary directory created at: " + temp_dir.name)

	# Run flex++ for header grammar first
	if run_flexcpp(temp_dir.name) != 0:
		print("Error running flex++.")
		exit(1)
	else:
		print("Successfully ran flex++.")

	# Rename base classes to snake case
	replace_str_in_files(glob.glob(temp_dir.name + "/*.h") + glob.glob(temp_dir.name + "/*.cc"), "lexerBase", "lexer_base")

		# Replace d_ member prefix to m_
	replace_str_in_files(glob.glob(temp_dir.name + "/*.h") + glob.glob(temp_dir.name + "/*.cc"), "d_matched", "m_matched")

	# Replace .ih include to .h
	replace_str_in_file(temp_dir.name + "/lexer.cc", "lexer.ih", "lexer.h")

	# Move the non-interface files to the src directory
	os.replace(temp_dir.name + "/lexer.cc", args.root_dir + "/src/lexer.cc")
	os.replace(temp_dir.name + "/lexer_base.h", args.root_dir + "/src/lexer_base.h")

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
	parser = argparse.ArgumentParser(description="Generate flex++ files.")
	parser.add_argument("--root-dir", help="Root directory of the project.")

	# Check if the root directory is valid.
	args = parser.parse_args()
	validate_args(args)
	main(args)