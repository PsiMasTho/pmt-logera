# Logera
	A Domain Specific Language used to convert log files to a csv file.

# Options
	-o = name of the output csv
	-d = directory containing a header and zero or more logs
	-m = manually select files

# Usage
	Logera -d logs/ -o data.csv # writes to data.csv
	Logera -d logs/ 			# writes to stdout
	Logera -m logs/header.lh logs/log1.txt logs/log2.txt -o data.csv

# Input files
	header:
		...
	logs:
		Values must not use characters '$' or ';' or '%'

# Dependencies
	CMake, Compiler that supports C++20 and Boost.test if tests are enabled.

# Building
	mkdir build && cd build
	cmake -S ../ -B .
	make -j$(nproc)

	Here are options that can be passed when invoking cmake:
	* -DBUILD_TYPE=[Release|DebugASAN|DebugGDB|DebugUBSAN|DebugCACHEGRIND|DebugLIBCPP]
	Builds the binaries with different flags to either improve performance or to help with
	debugging. The type is Release by default.

	* -DBUILD_TESTS=[ON|OFF]
	Whether to compile the unit tests which requires Boost.test. OFF by default.

# Modifying code:
	In the auto generated files by bisonc++ and flexc++, do *NOT* edit the lines under the lines beginning with a dollar sign, otherwise
	those files will get regenerated, overwriting your changes.