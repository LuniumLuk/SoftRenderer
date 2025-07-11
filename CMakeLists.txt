cmake_minimum_required(VERSION 3.10)
project(SoftRenderer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    add_definitions(-DSOURCE_DIR=\"${CMAKE_SOURCE_DIR}/\")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# Platform-specific settings
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    set(PLATFORM_LIBS gdi32)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Cocoa")
    set(PLATFORM_LIBS "")
else()
    set(PLATFORM_LIBS "")
endif()

# Include directories
include_directories(src src/core src/platform src/sample)

# Find source files
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp")
file(GLOB_RECURSE PLATFORM_SOURCES "src/platform/*.cpp")
file(GLOB_RECURSE SAMPLE_SOURCES "src/sample/*.cpp")

# Combine all sources
set(ALL_SOURCES ${CORE_SOURCES} ${PLATFORM_SOURCES} ${SAMPLE_SOURCES})

# Create executable
add_executable(viewer ${ALL_SOURCES})

# Link libraries
if(PLATFORM_LIBS)
    target_link_libraries(viewer ${PLATFORM_LIBS})
endif()

# # Optional: Add tests directory if it exists
# if(EXISTS "${CMAKE_SOURCE_DIR}/tests")
#     enable_testing()
#     add_subdirectory(tests)
# endif()
