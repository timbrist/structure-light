project(StructureLight)

# Set the C++ standard to C++11 for all targets in the project
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# requirement of cmake version
cmake_minimum_required(VERSION 3.5)

# SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# find required opencv
find_package(OpenCV REQUIRED)

# directory of opencv headers
include_directories(${OpenCV_INCLUDE_DIRS})
# Set the include directories
include_directories(include)
# Add the source files from the "src" folder
file(GLOB SOURCES "src/*.cpp")

# name of executable file and path of source file
add_executable(main main.cpp ${SOURCES})

# directory of opencv library
link_directories(${OpenCV_LIBRARY_DIRS})

# opencv libraries
target_link_libraries(main ${OpenCV_LIBS})
