# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\main.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\main.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\main.dir\flags.make

CMakeFiles\main.dir\main.cpp.obj: CMakeFiles\main.dir\flags.make
CMakeFiles\main.dir\main.cpp.obj: ..\main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main.dir/main.cpp.obj"
	C:\PROGRA~2\MICROS~2\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\main.dir\main.cpp.obj /FdCMakeFiles\main.dir\ /FS -c C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\main.cpp
<<

CMakeFiles\main.dir\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	C:\PROGRA~2\MICROS~2\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64\cl.exe > CMakeFiles\main.dir\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\main.cpp
<<

CMakeFiles\main.dir\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	C:\PROGRA~2\MICROS~2\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\main.dir\main.cpp.s /c C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\main.cpp
<<

# Object files for target main
main_OBJECTS = \
"CMakeFiles\main.dir\main.cpp.obj"

# External object files for target main
main_EXTERNAL_OBJECTS =

main.exe: CMakeFiles\main.dir\main.cpp.obj
main.exe: CMakeFiles\main.dir\build.make
main.exe: algoapi.lib
main.exe: algospi.lib
main.exe: C:\local\boost_1_68_0\lib64-msvc-14.0\boost_system-vc140-mt-gd-x64-1_68.lib
main.exe: C:\local\boost_1_68_0\lib64-msvc-14.0\boost_thread-vc140-mt-gd-x64-1_68.lib
main.exe: C:\local\boost_1_68_0\lib64-msvc-14.0\boost_chrono-vc140-mt-gd-x64-1_68.lib
main.exe: C:\local\boost_1_68_0\lib64-msvc-14.0\boost_date_time-vc140-mt-gd-x64-1_68.lib
main.exe: C:\local\boost_1_68_0\lib64-msvc-14.0\boost_atomic-vc140-mt-gd-x64-1_68.lib
main.exe: CMakeFiles\main.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable main.exe"
	"C:\Program Files\CMake\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\main.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\10.0.17763.0\x64\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\10.0.17763.0\x64\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64\link.exe /nologo @CMakeFiles\main.dir\objects1.rsp @<<
 /out:main.exe /implib:main.lib /pdb:C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\cmake-build-debug\main.pdb /version:0.0  /machine:x64 /debug /INCREMENTAL /subsystem:console   -LIBPATH:\usr\local\lib64  -LIBPATH:\usr\local\lib  algoapi.lib algospi.lib curlpp.lib OpenSSL::SSL.lib OpenSSL::Crypto.lib C:\local\boost_1_68_0\lib64-msvc-14.0\boost_system-vc140-mt-gd-x64-1_68.lib C:\local\boost_1_68_0\lib64-msvc-14.0\boost_thread-vc140-mt-gd-x64-1_68.lib C:\local\boost_1_68_0\lib64-msvc-14.0\boost_chrono-vc140-mt-gd-x64-1_68.lib C:\local\boost_1_68_0\lib64-msvc-14.0\boost_date_time-vc140-mt-gd-x64-1_68.lib C:\local\boost_1_68_0\lib64-msvc-14.0\boost_atomic-vc140-mt-gd-x64-1_68.lib curl.lib pthread.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\main.dir\build: main.exe

.PHONY : CMakeFiles\main.dir\build

CMakeFiles\main.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\main.dir\cmake_clean.cmake
.PHONY : CMakeFiles\main.dir\clean

CMakeFiles\main.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\cmake-build-debug C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\cmake-build-debug C:\Users\VickuGu\Desktop\AlgoAPI-develop\AlgoAPI-develop\Algoapi_Linux\cmake-build-debug\CMakeFiles\main.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\main.dir\depend

