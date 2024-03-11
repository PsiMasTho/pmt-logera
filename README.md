# Logera
A language for logging variables and their associated attributes over time. Intended for logging daily
training, but can be used for other kinds of logging. The main focus of this repository is the 'logera-to-csv'
tool, which verifies and converts logera logs to csv files, though the source code for parsing logera files
is seperated into a library in the lib/ and include/ folders.

## Input
There are two kinds of input files in Logera.
* Header(s):
    Contain (only) declarations for attributes and variables, and assigns
    attributes to vairables. Attrubutes may contain ECMAscript regex expressions
    for the kind of values they accept. Variables may contain a list of attributes that
    are associated with them.

    Declarations can be split across multiple files, e.g. attributes in one file and variables in another.
    However, declarations and entries must not be mixed in the same file.

    Values may not contain: Line breaks, spaces, tabs, double-quotes, '$' or ';' or '%'

    Variable names may not contain all of the above and may not contain commas.

    Attribue names have the same restrictions as variable names.

#### Example file containing declarations for a workout program
```
    # Declaring attributes
attr: reps     ; [0-9]{1,2}s[0-9]{1,3}r  ; [0-9]+(,[0-9]+)*        ;
attr: weight   ; [0-9]+(\.[0-9]{1,3})?kg ; [0-9]+(\.[0-9]{1,3})?lb ;
attr: time     ; [0-9]{2}\:[0-9]{2}      ;
attr: distance ; [0-9]+km                ; [0-9]+m                 ;
#...

    # Declaring variables
var: dips                  ; reps ; weight ;           rpe ;
var: bench_press           ; reps ; weight ; incline ; rpe ;
var: db_tri_ext            ; reps ; weight ;           rpe ;
# ...
```

* Logs: Must start with the date yyyy-mm-dd, and contain zero or more entries for that day.
  An entry is a variable name, followed by one or more lines of attributes and their values.

#### Example log
```
2022-09-16

bw:
    weight 71kg;

dips:
    reps 2s10r; rpe 8;              # bodyweight
    weight 5kg; reps 3s8r;  rpe 10;

bench_press:
    weight 60kg; reps 4s2r; incline 15deg;
    weight 100kg; reps 5,4,4,1; incline 15deg;

treadmill:
    time 10:00;
# ...
```

For both types of files, lines starting with # are comments.

For more examples see the example/ folder.

## Useful flags
### Flags
* -h, --help: Display the full list of available flags and their descriptions.
* -s, --sort-by-width: Sort the output by the width of the entries.
* -a, --align: Pads the output so that all columns are aligned.

### Optional checks
By default, log files are expected to be named in increasing lexicographical order with respect to the dates, e.g. log_0001.log, log_0002.log, log_0003.log, etc.
and 'logera-to-csv' will emit an error if the sequence of file names does not align with the chronological order of the dates in the logs. This feature is designed to detect errors in file naming or date typos. However, you can disable this error notification using the '--Wno-unordered-dates' flag.

Declarations are also expected to be in sorted order, as it makes it easier to find them.
By default, 'logera-to-csv' will treat unsorted declarations as an error, but this can be surpressed with the '--Wno-unordered-decls' flag.

## Building (linux)
    mkdir build && cd build
    cmake -S ../ -B .
    make
