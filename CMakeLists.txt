cmake_minimum_required(VERSION 2.8.4)

set (CMAKE_CXX_STANDARD 11)

project(game-engine-dev-tools)

# Add platform specific definitions
if(IOS)
	message(STATUS "Compiling for iOS")
	add_definitions(-D__IOS__)
	set(PLATFORM_NAME "ios")
elseif(ANDROID)
	message(STATUS "Compiling for Android")
	add_definitions(-D__ANDROID__)
	set(PLATFORM_NAME "android")
elseif(UNIX AND APPLE)
	message(STATUS "Compiling for macOS")
	add_definitions(-D__MACOS__)
	set(PLATFORM_NAME "macos")
elseif(UNIX AND NOT APPLE)
	message(STATUS "Compiling for Linux")
	add_definitions(-D__LINUX__)
	set(LINUX TRUE)
	set(PLATFORM_NAME "linux")
endif()

# Output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# GameEngine dependency static library
add_subdirectory(external/game-engine)

# File Converter
add_subdirectory(DAE2JMP)