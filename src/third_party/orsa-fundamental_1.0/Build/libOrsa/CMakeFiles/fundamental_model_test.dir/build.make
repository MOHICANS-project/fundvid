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
include libOrsa/CMakeFiles/fundamental_model_test.dir/depend.make

# Include the progress variables for this target.
include libOrsa/CMakeFiles/fundamental_model_test.dir/progress.make

# Include the compile flags for this target's objects.
include libOrsa/CMakeFiles/fundamental_model_test.dir/flags.make

libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o: libOrsa/CMakeFiles/fundamental_model_test.dir/flags.make
libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o: /home/nicola/Downloads/orsa-fundamental_1.0/src/libOrsa/fundamental_model_test.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicola/Downloads/orsa-fundamental_1.0/Build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o"
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/libOrsa && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o -c /home/nicola/Downloads/orsa-fundamental_1.0/src/libOrsa/fundamental_model_test.cpp

libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.i"
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/libOrsa && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicola/Downloads/orsa-fundamental_1.0/src/libOrsa/fundamental_model_test.cpp > CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.i

libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.s"
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/libOrsa && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicola/Downloads/orsa-fundamental_1.0/src/libOrsa/fundamental_model_test.cpp -o CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.s

libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o.requires:
.PHONY : libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o.requires

libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o.provides: libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o.requires
	$(MAKE) -f libOrsa/CMakeFiles/fundamental_model_test.dir/build.make libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o.provides.build
.PHONY : libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o.provides

libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o.provides.build: libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o

# Object files for target fundamental_model_test
fundamental_model_test_OBJECTS = \
"CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o"

# External object files for target fundamental_model_test
fundamental_model_test_EXTERNAL_OBJECTS =

libOrsa/fundamental_model_test: libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o
libOrsa/fundamental_model_test: libOrsa/CMakeFiles/fundamental_model_test.dir/build.make
libOrsa/fundamental_model_test: libOrsa/libOrsa.a
libOrsa/fundamental_model_test: third_party/CppUnitLite/libCppUnitLite.a
libOrsa/fundamental_model_test: extras/libNumerics/libNumerics.a
libOrsa/fundamental_model_test: libOrsa/CMakeFiles/fundamental_model_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable fundamental_model_test"
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/libOrsa && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fundamental_model_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libOrsa/CMakeFiles/fundamental_model_test.dir/build: libOrsa/fundamental_model_test
.PHONY : libOrsa/CMakeFiles/fundamental_model_test.dir/build

libOrsa/CMakeFiles/fundamental_model_test.dir/requires: libOrsa/CMakeFiles/fundamental_model_test.dir/fundamental_model_test.cpp.o.requires
.PHONY : libOrsa/CMakeFiles/fundamental_model_test.dir/requires

libOrsa/CMakeFiles/fundamental_model_test.dir/clean:
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build/libOrsa && $(CMAKE_COMMAND) -P CMakeFiles/fundamental_model_test.dir/cmake_clean.cmake
.PHONY : libOrsa/CMakeFiles/fundamental_model_test.dir/clean

libOrsa/CMakeFiles/fundamental_model_test.dir/depend:
	cd /home/nicola/Downloads/orsa-fundamental_1.0/Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nicola/Downloads/orsa-fundamental_1.0/src /home/nicola/Downloads/orsa-fundamental_1.0/src/libOrsa /home/nicola/Downloads/orsa-fundamental_1.0/Build /home/nicola/Downloads/orsa-fundamental_1.0/Build/libOrsa /home/nicola/Downloads/orsa-fundamental_1.0/Build/libOrsa/CMakeFiles/fundamental_model_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libOrsa/CMakeFiles/fundamental_model_test.dir/depend

