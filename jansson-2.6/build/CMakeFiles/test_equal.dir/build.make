# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.1

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.1.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.1.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tulindanilcom/Downloads/jansson-2.6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tulindanilcom/Downloads/jansson-2.6/build

# Include any dependencies generated for this target.
include CMakeFiles/test_equal.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_equal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_equal.dir/flags.make

CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o: CMakeFiles/test_equal.dir/flags.make
CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o: ../test/suites/api/test_equal.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/tulindanilcom/Downloads/jansson-2.6/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o   -c /Users/tulindanilcom/Downloads/jansson-2.6/test/suites/api/test_equal.c

CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/tulindanilcom/Downloads/jansson-2.6/test/suites/api/test_equal.c > CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.i

CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/tulindanilcom/Downloads/jansson-2.6/test/suites/api/test_equal.c -o CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.s

CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o.requires:
.PHONY : CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o.requires

CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o.provides: CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o.requires
	$(MAKE) -f CMakeFiles/test_equal.dir/build.make CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o.provides.build
.PHONY : CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o.provides

CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o.provides.build: CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o

# Object files for target test_equal
test_equal_OBJECTS = \
"CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o"

# External object files for target test_equal
test_equal_EXTERNAL_OBJECTS =

bin/test_equal: CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o
bin/test_equal: CMakeFiles/test_equal.dir/build.make
bin/test_equal: lib/libjansson.a
bin/test_equal: CMakeFiles/test_equal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable bin/test_equal"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_equal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_equal.dir/build: bin/test_equal
.PHONY : CMakeFiles/test_equal.dir/build

CMakeFiles/test_equal.dir/requires: CMakeFiles/test_equal.dir/test/suites/api/test_equal.c.o.requires
.PHONY : CMakeFiles/test_equal.dir/requires

CMakeFiles/test_equal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_equal.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_equal.dir/clean

CMakeFiles/test_equal.dir/depend:
	cd /Users/tulindanilcom/Downloads/jansson-2.6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tulindanilcom/Downloads/jansson-2.6 /Users/tulindanilcom/Downloads/jansson-2.6 /Users/tulindanilcom/Downloads/jansson-2.6/build /Users/tulindanilcom/Downloads/jansson-2.6/build /Users/tulindanilcom/Downloads/jansson-2.6/build/CMakeFiles/test_equal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_equal.dir/depend

