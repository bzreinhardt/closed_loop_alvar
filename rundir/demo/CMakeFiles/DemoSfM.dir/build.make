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
include demo/CMakeFiles/DemoSfM.dir/depend.make

# Include the progress variables for this target.
include demo/CMakeFiles/DemoSfM.dir/progress.make

# Include the compile flags for this target's objects.
include demo/CMakeFiles/DemoSfM.dir/flags.make

demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o: demo/CMakeFiles/DemoSfM.dir/flags.make
demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o: ../demo/DemoSfM.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/demo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o -c /home/bzreinha/newalvar/alvar-2.0.0-src/demo/DemoSfM.cpp

demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DemoSfM.dir/DemoSfM.cpp.i"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bzreinha/newalvar/alvar-2.0.0-src/demo/DemoSfM.cpp > CMakeFiles/DemoSfM.dir/DemoSfM.cpp.i

demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DemoSfM.dir/DemoSfM.cpp.s"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bzreinha/newalvar/alvar-2.0.0-src/demo/DemoSfM.cpp -o CMakeFiles/DemoSfM.dir/DemoSfM.cpp.s

demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o.requires:
.PHONY : demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o.requires

demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o.provides: demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o.requires
	$(MAKE) -f demo/CMakeFiles/DemoSfM.dir/build.make demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o.provides.build
.PHONY : demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o.provides

demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o.provides.build: demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o

# Object files for target DemoSfM
DemoSfM_OBJECTS = \
"CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o"

# External object files for target DemoSfM
DemoSfM_EXTERNAL_OBJECTS =

demo/demosfm: demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o
demo/demosfm: demo/CMakeFiles/DemoSfM.dir/build.make
demo/demosfm: /usr/lib/libopencv_video.so
demo/demosfm: /usr/lib/libopencv_ml.so
demo/demosfm: /usr/lib/libopencv_legacy.so
demo/demosfm: /usr/lib/libopencv_imgproc.so
demo/demosfm: /usr/lib/libopencv_highgui.so
demo/demosfm: /usr/lib/libopencv_flann.so
demo/demosfm: /usr/lib/libopencv_features2d.so
demo/demosfm: /usr/lib/libopencv_core.so
demo/demosfm: /usr/lib/libopencv_calib3d.so
demo/demosfm: src/libalvar200.so
demo/demosfm: 3rdparty/libTinyXML.a
demo/demosfm: src/platform/libalvarplatform200.so
demo/demosfm: /usr/lib/libosgViewer.so
demo/demosfm: /usr/lib/libosgGA.so
demo/demosfm: /usr/lib/libosgDB.so
demo/demosfm: /usr/lib/libosgUtil.so
demo/demosfm: /usr/lib/libosgText.so
demo/demosfm: /usr/lib/libosg.so
demo/demosfm: /usr/lib/libOpenThreads.so
demo/demosfm: sample/libSharedSamples.a
demo/demosfm: src/libalvar200.so
demo/demosfm: /usr/lib/libopencv_video.so
demo/demosfm: /usr/lib/libopencv_ml.so
demo/demosfm: /usr/lib/libopencv_legacy.so
demo/demosfm: /usr/lib/libopencv_imgproc.so
demo/demosfm: /usr/lib/libopencv_highgui.so
demo/demosfm: /usr/lib/libopencv_flann.so
demo/demosfm: /usr/lib/libopencv_features2d.so
demo/demosfm: /usr/lib/libopencv_core.so
demo/demosfm: /usr/lib/libopencv_calib3d.so
demo/demosfm: 3rdparty/libTinyXML.a
demo/demosfm: demo/CMakeFiles/DemoSfM.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable demosfm"
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/demo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DemoSfM.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
demo/CMakeFiles/DemoSfM.dir/build: demo/demosfm
.PHONY : demo/CMakeFiles/DemoSfM.dir/build

demo/CMakeFiles/DemoSfM.dir/requires: demo/CMakeFiles/DemoSfM.dir/DemoSfM.cpp.o.requires
.PHONY : demo/CMakeFiles/DemoSfM.dir/requires

demo/CMakeFiles/DemoSfM.dir/clean:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/demo && $(CMAKE_COMMAND) -P CMakeFiles/DemoSfM.dir/cmake_clean.cmake
.PHONY : demo/CMakeFiles/DemoSfM.dir/clean

demo/CMakeFiles/DemoSfM.dir/depend:
	cd /home/bzreinha/newalvar/alvar-2.0.0-src/rundir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bzreinha/newalvar/alvar-2.0.0-src /home/bzreinha/newalvar/alvar-2.0.0-src/demo /home/bzreinha/newalvar/alvar-2.0.0-src/rundir /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/demo /home/bzreinha/newalvar/alvar-2.0.0-src/rundir/demo/CMakeFiles/DemoSfM.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : demo/CMakeFiles/DemoSfM.dir/depend

