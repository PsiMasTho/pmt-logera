# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pmt/repos/pmt-logera

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pmt/repos/pmt-logera/test

# Include any dependencies generated for this target.
include src/to_csv/CMakeFiles/logera-to-csv.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/to_csv/CMakeFiles/logera-to-csv.dir/compiler_depend.make

# Include the progress variables for this target.
include src/to_csv/CMakeFiles/logera-to-csv.dir/progress.make

# Include the compile flags for this target's objects.
include src/to_csv/CMakeFiles/logera-to-csv.dir/flags.make

src/to_csv/CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o: src/to_csv/CMakeFiles/logera-to-csv.dir/flags.make
src/to_csv/CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o: /home/pmt/repos/pmt-logera/src/to_csv/csv_emitter.cc
src/to_csv/CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o: src/to_csv/CMakeFiles/logera-to-csv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pmt/repos/pmt-logera/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/to_csv/CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/to_csv/CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o -MF CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o.d -o CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o -c /home/pmt/repos/pmt-logera/src/to_csv/csv_emitter.cc

src/to_csv/CMakeFiles/logera-to-csv.dir/csv_emitter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/logera-to-csv.dir/csv_emitter.cc.i"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pmt/repos/pmt-logera/src/to_csv/csv_emitter.cc > CMakeFiles/logera-to-csv.dir/csv_emitter.cc.i

src/to_csv/CMakeFiles/logera-to-csv.dir/csv_emitter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/logera-to-csv.dir/csv_emitter.cc.s"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pmt/repos/pmt-logera/src/to_csv/csv_emitter.cc -o CMakeFiles/logera-to-csv.dir/csv_emitter.cc.s

src/to_csv/CMakeFiles/logera-to-csv.dir/main.cc.o: src/to_csv/CMakeFiles/logera-to-csv.dir/flags.make
src/to_csv/CMakeFiles/logera-to-csv.dir/main.cc.o: /home/pmt/repos/pmt-logera/src/to_csv/main.cc
src/to_csv/CMakeFiles/logera-to-csv.dir/main.cc.o: src/to_csv/CMakeFiles/logera-to-csv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pmt/repos/pmt-logera/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/to_csv/CMakeFiles/logera-to-csv.dir/main.cc.o"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/to_csv/CMakeFiles/logera-to-csv.dir/main.cc.o -MF CMakeFiles/logera-to-csv.dir/main.cc.o.d -o CMakeFiles/logera-to-csv.dir/main.cc.o -c /home/pmt/repos/pmt-logera/src/to_csv/main.cc

src/to_csv/CMakeFiles/logera-to-csv.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/logera-to-csv.dir/main.cc.i"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pmt/repos/pmt-logera/src/to_csv/main.cc > CMakeFiles/logera-to-csv.dir/main.cc.i

src/to_csv/CMakeFiles/logera-to-csv.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/logera-to-csv.dir/main.cc.s"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pmt/repos/pmt-logera/src/to_csv/main.cc -o CMakeFiles/logera-to-csv.dir/main.cc.s

src/to_csv/CMakeFiles/logera-to-csv.dir/program_opts.cc.o: src/to_csv/CMakeFiles/logera-to-csv.dir/flags.make
src/to_csv/CMakeFiles/logera-to-csv.dir/program_opts.cc.o: /home/pmt/repos/pmt-logera/src/to_csv/program_opts.cc
src/to_csv/CMakeFiles/logera-to-csv.dir/program_opts.cc.o: src/to_csv/CMakeFiles/logera-to-csv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pmt/repos/pmt-logera/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/to_csv/CMakeFiles/logera-to-csv.dir/program_opts.cc.o"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/to_csv/CMakeFiles/logera-to-csv.dir/program_opts.cc.o -MF CMakeFiles/logera-to-csv.dir/program_opts.cc.o.d -o CMakeFiles/logera-to-csv.dir/program_opts.cc.o -c /home/pmt/repos/pmt-logera/src/to_csv/program_opts.cc

src/to_csv/CMakeFiles/logera-to-csv.dir/program_opts.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/logera-to-csv.dir/program_opts.cc.i"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pmt/repos/pmt-logera/src/to_csv/program_opts.cc > CMakeFiles/logera-to-csv.dir/program_opts.cc.i

src/to_csv/CMakeFiles/logera-to-csv.dir/program_opts.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/logera-to-csv.dir/program_opts.cc.s"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pmt/repos/pmt-logera/src/to_csv/program_opts.cc -o CMakeFiles/logera-to-csv.dir/program_opts.cc.s

# Object files for target logera-to-csv
logera__to__csv_OBJECTS = \
"CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o" \
"CMakeFiles/logera-to-csv.dir/main.cc.o" \
"CMakeFiles/logera-to-csv.dir/program_opts.cc.o"

# External object files for target logera-to-csv
logera__to__csv_EXTERNAL_OBJECTS =

src/to_csv/logera-to-csv: src/to_csv/CMakeFiles/logera-to-csv.dir/csv_emitter.cc.o
src/to_csv/logera-to-csv: src/to_csv/CMakeFiles/logera-to-csv.dir/main.cc.o
src/to_csv/logera-to-csv: src/to_csv/CMakeFiles/logera-to-csv.dir/program_opts.cc.o
src/to_csv/logera-to-csv: src/to_csv/CMakeFiles/logera-to-csv.dir/build.make
src/to_csv/logera-to-csv: src/liblogera.so
src/to_csv/logera-to-csv: /usr/lib/x86_64-linux-gnu/libfmt.so.9.1.0
src/to_csv/logera-to-csv: src/to_csv/CMakeFiles/logera-to-csv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/pmt/repos/pmt-logera/test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable logera-to-csv"
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/logera-to-csv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/to_csv/CMakeFiles/logera-to-csv.dir/build: src/to_csv/logera-to-csv
.PHONY : src/to_csv/CMakeFiles/logera-to-csv.dir/build

src/to_csv/CMakeFiles/logera-to-csv.dir/clean:
	cd /home/pmt/repos/pmt-logera/test/src/to_csv && $(CMAKE_COMMAND) -P CMakeFiles/logera-to-csv.dir/cmake_clean.cmake
.PHONY : src/to_csv/CMakeFiles/logera-to-csv.dir/clean

src/to_csv/CMakeFiles/logera-to-csv.dir/depend:
	cd /home/pmt/repos/pmt-logera/test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pmt/repos/pmt-logera /home/pmt/repos/pmt-logera/src/to_csv /home/pmt/repos/pmt-logera/test /home/pmt/repos/pmt-logera/test/src/to_csv /home/pmt/repos/pmt-logera/test/src/to_csv/CMakeFiles/logera-to-csv.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/to_csv/CMakeFiles/logera-to-csv.dir/depend

