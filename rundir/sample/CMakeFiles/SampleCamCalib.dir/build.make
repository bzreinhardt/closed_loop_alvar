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

# Include any dependencies generated for this target.
include sample/CMakeFiles/SampleCamCalib.dir/depend.make

# Include the progress variables for this target.
include sample/CMakeFiles/SampleCamCalib.dir/progress.make

# Include the compile flags for this target's objects.
include sample/CMakeFiles/SampleCamCalib.dir/flags.make

sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o: sample/CMakeFiles/SampleCamCalib.dir/flags.make
sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o: ../sample/SampleCamCalib.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o -c /home/bzreinha/newalvar/alvar-2.0.0-src/sample/SampleCamCalib.cpp

sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.i"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bzreinha/newalvar/alvar-2.0.0-src/sample/SampleCamCalib.cpp > CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.i

sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.s"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bzreinha/newalvar/alvar-2.0.0-src/sample/SampleCamCalib.cpp -o CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.s

sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o.requires:
.PHONY : sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o.requires

sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o.provides: sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o.requires
	$(MAKE) -f sample/CMakeFiles/SampleCamCalib.dir/build.make sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o.provides.build
.PHONY : sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o.provides

sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o.provides.build: sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o

# Object files for target SampleCamCalib
SampleCamCalib_OBJECTS = \
"CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o"

# External object files for target SampleCamCalib
SampleCamCalib_EXTERNAL_OBJECTS =

sample/samplecamcalib: sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o
sample/samplecamcalib: sample/CMakeFiles/SampleCamCalib.dir/build.make
sample/samplecamcalib: /usr/lib/libopencv_video.so
sample/samplecamcalib: /usr/lib/libopencv_ml.so
sample/samplecamcalib: /usr/lib/libopencv_legacy.so
sample/samplecamcalib: /usr/lib/libopencv_imgproc.so
sample/samplecamcalib: /usr/lib/libopencv_highgui.so
sample/samplecamcalib: /usr/lib/libopencv_flann.so
sample/samplecamcalib: /usr/lib/libopencv_features2d.so
sample/samplecamcalib: /usr/lib/libopencv_core.so
sample/samplecamcalib: /usr/lib/libopencv_calib3d.so
sample/samplecamcalib: src/libalvar200.so
sample/samplecamcalib: 3rdparty/libTinyXML.a
sample/samplecamcalib: src/platform/libalvarplatform200.so
sample/samplecamcalib: /usr/local/lib/libzmq.so
sample/samplecamcalib: sample/libSharedSamples.a
sample/samplecamcalib: src/libalvar200.so
sample/samplecamcalib: /usr/lib/libopencv_video.so
sample/samplecamcalib: /usr/lib/libopencv_ml.so
sample/samplecamcalib: /usr/lib/libopencv_legacy.so
sample/samplecamcalib: /usr/lib/libopencv_imgproc.so
sample/samplecamcalib: /usr/lib/libopencv_highgui.so
sample/samplecamcalib: /usr/lib/libopencv_flann.so
sample/samplecamcalib: /usr/lib/libopencv_features2d.so
sample/samplecamcalib: /usr/lib/libopencv_core.so
sample/samplecamcalib: /usr/lib/libopencv_calib3d.so
sample/samplecamcalib: 3rdparty/libTinyXML.a
sample/samplecamcalib: sample/CMakeFiles/SampleCamCalib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable samplecamcalib"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SampleCamCalib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sample/CMakeFiles/SampleCamCalib.dir/build: sample/samplecamcalib
.PHONY : sample/CMakeFiles/SampleCamCalib.dir/build

sample/CMakeFiles/SampleCamCalib.dir/requires: sample/CMakeFiles/SampleCamCalib.dir/SampleCamCalib.cpp.o.requires
.PHONY : sample/CMakeFiles/SampleCamCalib.dir/requires

sample/CMakeFiles/SampleCamCalib.dir/clean:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && $(CMAKE_COMMAND) -P CMakeFiles/SampleCamCalib.dir/cmake_clean.cmake
.PHONY : sample/CMakeFiles/SampleCamCalib.dir/clean

sample/CMakeFiles/SampleCamCalib.dir/depend:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bzreinha/newalvar/alvar-2.0.0-src /home/bzreinha/newalvar/alvar-2.0.0-src/sample /home/bzreinha/newalvar/alvar-2.0.0-src/rundir /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample/CMakeFiles/SampleCamCalib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sample/CMakeFiles/SampleCamCalib.dir/depend

