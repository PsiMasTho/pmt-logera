# Logera
A library for parsing custom log files that contain variables and attributes
across time. Intended to be used for logging daily training, but can be used for
other kinds of logging.

## Input files
* Header:
    Contains declarations for attributes and variables, and assigns
    attributes to vairables. Attrubutes may contain ECMAscript regex expressions
    for the kind of values they accept. Must have a '.lh' extension.

    Values may not contain: Line breaks, spaces, tabs, double-quotes, '$' or ';' or '%'

    Variable names may not contain all of the above and may not contain commas.

    Attribue names have the same restrictions as variable names.

#### Example header for a workout program
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

* Logs: Must start with the date yyyy-mm-dd, contain variables and the values of their attributes for that day.

#### Example log
```
2022-09-16

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
```

## Dependencies
### To just compile logera:
* *CMake*
* Compiler that supports C++20
### If building tests:
* *Catch2*
### If modifying scanner and grammar files:
* *bisonc++* and/or *flexc++* to (re)generate scanners and parsers

## Building (linux)

    mkdir build && cd build
    cmake -S ../ -B .
    make -j$(nproc)


Here are options that can be passed when invoking cmake:
* -DBUILD_TESTS=[ON|OFF]
Whether to compile the unit tests which requires Catch2. OFF by default.
