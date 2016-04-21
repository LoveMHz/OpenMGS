cmake_minimum_required(VERSION 2.6)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Main LIB
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/lib_metalgearsolid_pc)

# Tools
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/source/tools/extract_dar)
