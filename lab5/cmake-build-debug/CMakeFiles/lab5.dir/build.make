# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.4\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Dyuga\Desktop\study_labs\os\lab5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Dyuga\Desktop\study_labs\os\lab5\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lab5.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lab5.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab5.dir/flags.make

CMakeFiles/lab5.dir/main.c.obj: CMakeFiles/lab5.dir/flags.make
CMakeFiles/lab5.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Dyuga\Desktop\study_labs\os\lab5\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lab5.dir/main.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\lab5.dir\main.c.obj -c C:\Users\Dyuga\Desktop\study_labs\os\lab5\main.c

CMakeFiles/lab5.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab5.dir/main.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Dyuga\Desktop\study_labs\os\lab5\main.c > CMakeFiles\lab5.dir\main.c.i

CMakeFiles/lab5.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab5.dir/main.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Dyuga\Desktop\study_labs\os\lab5\main.c -o CMakeFiles\lab5.dir\main.c.s

CMakeFiles/lab5.dir/FileStrSearch.c.obj: CMakeFiles/lab5.dir/flags.make
CMakeFiles/lab5.dir/FileStrSearch.c.obj: ../FileStrSearch.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Dyuga\Desktop\study_labs\os\lab5\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lab5.dir/FileStrSearch.c.obj"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\lab5.dir\FileStrSearch.c.obj -c C:\Users\Dyuga\Desktop\study_labs\os\lab5\FileStrSearch.c

CMakeFiles/lab5.dir/FileStrSearch.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab5.dir/FileStrSearch.c.i"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Dyuga\Desktop\study_labs\os\lab5\FileStrSearch.c > CMakeFiles\lab5.dir\FileStrSearch.c.i

CMakeFiles/lab5.dir/FileStrSearch.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab5.dir/FileStrSearch.c.s"
	C:\PROGRA~2\MINGW-~1\I686-8~1.0-P\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Dyuga\Desktop\study_labs\os\lab5\FileStrSearch.c -o CMakeFiles\lab5.dir\FileStrSearch.c.s

# Object files for target lab5
lab5_OBJECTS = \
"CMakeFiles/lab5.dir/main.c.obj" \
"CMakeFiles/lab5.dir/FileStrSearch.c.obj"

# External object files for target lab5
lab5_EXTERNAL_OBJECTS =

lab5.exe: CMakeFiles/lab5.dir/main.c.obj
lab5.exe: CMakeFiles/lab5.dir/FileStrSearch.c.obj
lab5.exe: CMakeFiles/lab5.dir/build.make
lab5.exe: CMakeFiles/lab5.dir/linklibs.rsp
lab5.exe: CMakeFiles/lab5.dir/objects1.rsp
lab5.exe: CMakeFiles/lab5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Dyuga\Desktop\study_labs\os\lab5\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable lab5.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lab5.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab5.dir/build: lab5.exe

.PHONY : CMakeFiles/lab5.dir/build

CMakeFiles/lab5.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab5.dir\cmake_clean.cmake
.PHONY : CMakeFiles/lab5.dir/clean

CMakeFiles/lab5.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Dyuga\Desktop\study_labs\os\lab5 C:\Users\Dyuga\Desktop\study_labs\os\lab5 C:\Users\Dyuga\Desktop\study_labs\os\lab5\cmake-build-debug C:\Users\Dyuga\Desktop\study_labs\os\lab5\cmake-build-debug C:\Users\Dyuga\Desktop\study_labs\os\lab5\cmake-build-debug\CMakeFiles\lab5.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lab5.dir/depend

