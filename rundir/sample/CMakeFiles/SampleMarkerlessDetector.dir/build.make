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
include sample/CMakeFiles/SampleMarkerlessDetector.dir/depend.make

# Include the progress variables for this target.
include sample/CMakeFiles/SampleMarkerlessDetector.dir/progress.make

# Include the compile flags for this target's objects.
include sample/CMakeFiles/SampleMarkerlessDetector.dir/flags.make

sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o: sample/CMakeFiles/SampleMarkerlessDetector.dir/flags.make
sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o: ../sample/SampleMarkerlessDetector.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o -c /home/bzreinha/newalvar/alvar-2.0.0-src/sample/SampleMarkerlessDetector.cpp

sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.i"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bzreinha/newalvar/alvar-2.0.0-src/sample/SampleMarkerlessDetector.cpp > CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.i

sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.s"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bzreinha/newalvar/alvar-2.0.0-src/sample/SampleMarkerlessDetector.cpp -o CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.s

sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o.requires:
.PHONY : sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o.requires

sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o.provides: sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o.requires
	$(MAKE) -f sample/CMakeFiles/SampleMarkerlessDetector.dir/build.make sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o.provides.build
.PHONY : sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o.provides

sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o.provides.build: sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o

# Object files for target SampleMarkerlessDetector
SampleMarkerlessDetector_OBJECTS = \
"CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o"

# External object files for target SampleMarkerlessDetector
SampleMarkerlessDetector_EXTERNAL_OBJECTS =

sample/samplemarkerlessdetector: sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o
sample/samplemarkerlessdetector: sample/CMakeFiles/SampleMarkerlessDetector.dir/build.make
sample/samplemarkerlessdetector: /usr/lib/libopencv_video.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_ml.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_legacy.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_imgproc.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_highgui.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_flann.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_features2d.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_core.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_calib3d.so
sample/samplemarkerlessdetector: src/libalvar200.so
sample/samplemarkerlessdetector: 3rdparty/libTinyXML.a
sample/samplemarkerlessdetector: src/platform/libalvarplatform200.so
sample/samplemarkerlessdetector: /usr/local/lib/libzmq.so
sample/samplemarkerlessdetector: sample/libSharedSamples.a
sample/samplemarkerlessdetector: sample/libSharedGlutViewer.a
sample/samplemarkerlessdetector: src/libalvar200.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_video.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_ml.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_legacy.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_imgproc.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_highgui.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_flann.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_features2d.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_core.so
sample/samplemarkerlessdetector: /usr/lib/libopencv_calib3d.so
sample/samplemarkerlessdetector: 3rdparty/libTinyXML.a
sample/samplemarkerlessdetector: /usr/lib/x86_64-linux-gnu/libGLU.so
sample/samplemarkerlessdetector: /usr/lib/x86_64-linux-gnu/libGL.so
sample/samplemarkerlessdetector: /usr/lib/x86_64-linux-gnu/libSM.so
sample/samplemarkerlessdetector: /usr/lib/x86_64-linux-gnu/libICE.so
sample/samplemarkerlessdetector: /usr/lib/x86_64-linux-gnu/libX11.so
sample/samplemarkerlessdetector: /usr/lib/x86_64-linux-gnu/libXext.so
sample/samplemarkerlessdetector: /usr/lib/x86_64-linux-gnu/libglut.so
sample/samplemarkerlessdetector: sample/CMakeFiles/SampleMarkerlessDetector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable samplemarkerlessdetector"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SampleMarkerlessDetector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sample/CMakeFiles/SampleMarkerlessDetector.dir/build: sample/samplemarkerlessdetector
.PHONY : sample/CMakeFiles/SampleMarkerlessDetector.dir/build

sample/CMakeFiles/SampleMarkerlessDetector.dir/requires: sample/CMakeFiles/SampleMarkerlessDetector.dir/SampleMarkerlessDetector.cpp.o.requires
.PHONY : sample/CMakeFiles/SampleMarkerlessDetector.dir/requires

sample/CMakeFiles/SampleMarkerlessDetector.dir/clean:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample && $(CMAKE_COMMAND) -P CMakeFiles/SampleMarkerlessDetector.dir/cmake_clean.cmake
.PHONY : sample/CMakeFiles/SampleMarkerlessDetector.dir/clean

sample/CMakeFiles/SampleMarkerlessDetector.dir/depend:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bzreinha/newalvar/alvar-2.0.0-src /home/bzreinha/newalvar/alvar-2.0.0-src/sample /home/bzreinha/newalvar/alvar-2.0.0-src/rundir /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/sample/CMakeFiles/SampleMarkerlessDetector.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sample/CMakeFiles/SampleMarkerlessDetector.dir/depend

