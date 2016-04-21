cmake_minimum_required(VERSION 2.6)

add_subdirectory(./../../lib_metalgearsolid_pc metalgearsolid_pc)

include_directories(${SHARED_LIB_INCLUDE_DIR})

set(SRCS src/extract_dar.c)
add_executable(extract_dar ${SRCS})
target_link_libraries(extract_dar metalgearsolid_pc)
