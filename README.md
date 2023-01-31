# Logera
A compiler that takes custom log files that contain variables and attributes
across time and compiles them into a csv file.

## Options
	-o = name of the output csv
	-d = directory containing a header and zero or more logs
	-m = manually select files

## Usage
	Logera -d logs/ -o data.csv # writes to data.csv

	Logera -d logs/ 			# writes to stdout

	Logera -m logs/header.lh logs/log1.txt logs/log2.txt -o data.csv

## Input files
* Header:
	Contains declarations for attributes and variables, and assigns
	attributes to vairables. Attrubutes may contain ECMAscript regex expressions
	for the kind of values they accept.

	Values may not contain: Line breaks, double-quotes or semicolons.
	Variable names may not contain all of the above and may not contain commas.
	Attribue names have the same restrictions as variable names.

#### Example header for a workout programme
```
    # Declaring attributes
>attr reps     ; [0-9]{1,2}s[0-9]{1,3}r  ; [0-9]+(,[0-9]+)*        ;
>attr weight   ; [0-9]+(\.[0-9]{1,3})?kg ; [0-9]+(\.[0-9]{1,3})?lb ;
>attr time     ; [0-9]{2}\:[0-9]{2}      ;
>attr distance ; [0-9]+km                ; [0-9]+m                 ;
#...

	# Declaring variables
>var dips                  ; reps ; weight ;           rpe ;
>var bench_press           ; reps ; weight ; incline ; rpe ;
>var db_tri_ext            ; reps ; weight ;           rpe ;
# ...
```

* Logs: Must start with the date dd/mm/yyyy, contain variables and the values of their attributes for that day. Values must not use characters '$' or ';' or '%'

#### Example log
```
09/10/2022

bw:
    weight 71kg;

dips:
    weight 0kg; reps 2s10r; rpe 8;
    weight 5kg; reps 3s8r;  rpe 10;

bench_press:
    weight 60kg; reps 4s2r; incline 15deg;
    weight 100kg; reps 5,4,4,1; incline 15deg;

treadmill:
    time 10:00;
# ...
```

For both types of files, lines starting with # are comments.

For more information see examples/ folder.

#### Example output
```
Date;Var;reps;weight;time;distance;energy;rpe;incline
09/10/2022;bw;;71kg;;;;;
09/10/2022;dips;2s10r;0kg;;;;8;
09/10/2022;dips;3s8r;5kg;;;;10;
09/10/2022;bench_press;4s2r;60kg;;;;;15deg
09/10/2022;bench_press;5,4,4,1;100kg;;;;;15deg
09/10/2022;btn_situps;10,10,10,10;2.5kg;;;;;
09/10/2022;btn_situps;5s10r;5kg;;;;;
09/10/2022;treadmill;;;10:00;;;;
11/10/2022;bw;;78kg;;;;;
11/10/2022;kcal;;;;;3500kcal;;
11/10/2022;leg_extension;5s10r;100kg;;;;10;
11/10/2022;shoulder_press;5s10r;60kg;;;;8;
11/10/2022;leg_curl;4,4,4,5;60kg;;;;7;
```

## Dependencies
### To just compile Logera:
* *CMake*
* *make*
* Compiler that supports C++20
### If building tests:
* *Boost.test*
### If modifying scanner and grammar files:
* *bisonc++* and/or *flexc++* to (re)generate scanners and parsers

## Building

	mkdir build && cd build
	cmake -S ../ -B .
	make -j$(nproc)


Here are options that can be passed when invoking cmake:
* -DBUILD_TYPE=[Release|DebugASAN|DebugGDB|DebugUBSAN|DebugCACHEGRIND|DebugLIBCPP]
Builds the binaries with different flags to either improve performance or to help with
debugging. The type is Release by default.

* -DBUILD_TESTS=[ON|OFF]
Whether to compile the unit tests which requires Boost.test. OFF by default.