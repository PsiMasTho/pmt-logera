NAME
	Logara - a Domain Specific Language used to convert workout logs to csv files.

SYNOPSIS
	Logara -[I|E <regex>] -d <directory> [-o <output.csv>]
	Logara -m <file.lh and files.txt...> [-o <output.csv>]

DESCRIPTION
	headers and logs should be saved with th

OPTIONS
	-o = name of the output csv
	-d = directory containing a header and zero or more logs
	-m = manually select files
	-I = inclusive filter (cannot use with exclusive) 
	-E = exclusive filter (cannot use with inclusive)

EXAMPLES
	Logara -d logs/ -o data.csv # writes to data.csv
	Logara -d logs/ 			# writes to stdout
	Logara -m logs/header.lh logs/log1.txt logs/log2.txt -o data.csv