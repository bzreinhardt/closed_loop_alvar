# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bzreinha/newalvar/alvar-2.0.0-src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bzreinha/newalvar/alvar-2.0.0-src/rundir

# Utility rule file for Documentation.

# Include the progress variables for this target.
include doc/CMakeFiles/Documentation.dir/progress.make

doc/CMakeFiles/Documentation:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/doc && /usr/bin/doxygen doxygen.config

Documentation: doc/CMakeFiles/Documentation
Documentation: doc/CMakeFiles/Documentation.dir/build.make
.PHONY : Documentation

# Rule to build all files generated by this target.
doc/CMakeFiles/Documentation.dir/build: Documentation
.PHONY : doc/CMakeFiles/Documentation.dir/build

doc/CMakeFiles/Documentation.dir/clean:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/doc && $(CMAKE_COMMAND) -P CMakeFiles/Documentation.dir/cmake_clean.cmake
.PHONY : doc/CMakeFiles/Documentation.dir/clean

doc/CMakeFiles/Documentation.dir/depend:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bzreinha/newalvar/alvar-2.0.0-src /home/bzreinha/newalvar/alvar-2.0.0-src/doc /home/bzreinha/newalvar/alvar-2.0.0-src/rundir /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/doc /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/doc/CMakeFiles/Documentation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/CMakeFiles/Documentation.dir/depend

