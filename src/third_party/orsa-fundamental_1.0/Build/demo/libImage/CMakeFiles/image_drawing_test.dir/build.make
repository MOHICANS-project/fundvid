# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nicola/Downloads/orsa-fundamental_1.0/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nicola/Downloads/orsa-fundamental_1.0/Build

# Include any dependencies generated for this target.
include demo/libImage/CMakeFiles/image_drawing_test.dir/depend.make

# Include the progress variables for this target.
include demo/libImage/CMakeFiles/image_drawing_test.dir/progress.make

# Include the compile flags for this target's objects.
include demo/libImage/CMakeFiles/image_drawing_test.dir/flags.make

demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o: demo/libImage/CMakeFiles/image_drawing_test.dir/flags.make
demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o: /home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_drawing_test.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicola/Downloads/orsa-fundamental_1.0/Build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o"
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/demo/libImage && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o -c /home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_drawing_test.cpp

demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.i"
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/demo/libImage && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_drawing_test.cpp > CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.i

demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.s"
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/demo/libImage && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage/image_drawing_test.cpp -o CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.s

demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o.requires:
.PHONY : demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o.requires

demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o.provides: demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o.requires
	$(MAKE) -f demo/libImage/CMakeFiles/image_drawing_test.dir/build.make demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o.provides.build
.PHONY : demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o.provides

demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o.provides.build: demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o

# Object files for target image_drawing_test
image_drawing_test_OBJECTS = \
"CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o"

# External object files for target image_drawing_test
image_drawing_test_EXTERNAL_OBJECTS =

demo/libImage/image_drawing_test: demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o
demo/libImage/image_drawing_test: demo/libImage/CMakeFiles/image_drawing_test.dir/build.make
demo/libImage/image_drawing_test: demo/libImage/libimage.a
demo/libImage/image_drawing_test: third_party/CppUnitLite/libCppUnitLite.a
demo/libImage/image_drawing_test: /usr/lib/x86_64-linux-gnu/libpng.so
demo/libImage/image_drawing_test: /usr/lib/x86_64-linux-gnu/libz.so
demo/libImage/image_drawing_test: /usr/lib/x86_64-linux-gnu/libjpeg.so
demo/libImage/image_drawing_test: demo/libImage/CMakeFiles/image_drawing_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable image_drawing_test"
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/demo/libImage && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/image_drawing_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
demo/libImage/CMakeFiles/image_drawing_test.dir/build: demo/libImage/image_drawing_test
.PHONY : demo/libImage/CMakeFiles/image_drawing_test.dir/build

demo/libImage/CMakeFiles/image_drawing_test.dir/requires: demo/libImage/CMakeFiles/image_drawing_test.dir/image_drawing_test.cpp.o.requires
.PHONY : demo/libImage/CMakeFiles/image_drawing_test.dir/requires

demo/libImage/CMakeFiles/image_drawing_test.dir/clean:
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/demo/libImage && $(CMAKE_COMMAND) -P CMakeFiles/image_drawing_test.dir/cmake_clean.cmake
.PHONY : demo/libImage/CMakeFiles/image_drawing_test.dir/clean

demo/libImage/CMakeFiles/image_drawing_test.dir/depend:
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nicola/Downloads/orsa-fundamental_1.0/src /home/nicola/Downloads/orsa-fundamental_1.0/src/demo/libImage /home/nicola/Downloads/orsa-fundamental_1.0/Build /home/nicola/Downloads/orsa-fundamental_1.0/Build/demo/libImage /home/nicola/Downloads/orsa-fundamental_1.0/Build/demo/libImage/CMakeFiles/image_drawing_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : demo/libImage/CMakeFiles/image_drawing_test.dir/depend

