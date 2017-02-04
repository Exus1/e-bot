cmake_minimum_required(VERSION 3.2.2)

project(e-bot)

# e-bot version set
set(E-BOT_VERSION_MAJOR "0")
set(E-BOT_VERSION_MINIOR "1")
set(E-BOT_VERSION_BUILD "alpha")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RELEASE")
endif()

# GCC Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

#CXX Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,-rpath='$ORIGIN/libs'")

# Release CXX Flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Debug CXX Flags
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

file(MAKE_DIRECTORY Modules)

#Library output folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "Modules")

# Build architecture
if(NOT DEFINED BUILD_x86)
	set(BUILD_ARCH "x86_64")
	set(BUILD_ARCH_OPTION "-m64")
else()
	set(BUILD_ARCH "x86")
	set(BUILD_ARCH_OPTION "-m32")
endif()

# Set build architecture
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_ARCH_OPTION}")

# e-bot version 
set(E-BOT_VERSION "${E-BOT_VERSION_MAJOR}.${E-BOT_VERSION_MINIOR}_${E-BOT_VERSION_BUILD}_${BUILD_ARCH}-${CMAKE_BUILD_TYPE}")


# TeamSpeak3-C-Query-API headers
include_directories(
	TeamSpeak3-C-Query-API_headers
)

# e-bot include direcotory
include_directories(
	src/includes
)

# Library folder
link_directories(${CMAKE_SOURCE_DIR}/libs)

# Set of e-bot cpp files
set(E-BOT_SRCS
	src/main.cpp
	src/Modules_manager.cpp
	src/Module_thread.cpp
	src/ConfigFile.cpp
)

# Add executable
add_executable(e-bot ${E-BOT_SRCS})


# Set executable Name
set_target_properties(e-bot PROPERTIES OUTPUT_NAME "e-bot-${E-BOT_VERSION}")

# Link TeamSpeak3-C-Query-API library
target_link_libraries(e-bot teamspeak-api-${BUILD_ARCH})

# Link required libraries
target_link_libraries(e-bot dl)
target_link_libraries(e-bot pthread)


add_library(modul MODULE src/example_library.cpp)
SET_TARGET_PROPERTIES(modul PROPERTIES PREFIX "")
 SET_TARGET_PROPERTIES(modul PROPERTIES SUFFIX ".emodule")

