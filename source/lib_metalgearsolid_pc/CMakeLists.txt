cmake_minimum_required(VERSION 2.6)

set(SHARED_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SHARED_LIB_INCLUDE_DIR ${SHARED_LIB_INCLUDE_DIR} PARENT_SCOPE)

if(NOT TARGET metalgearsolid_pc)
  include_directories(${SHARED_LIB_INCLUDE_DIR})
  set(LIB_SRCS ./src/mgs_archive.c)

  add_library(metalgearsolid_pc STATIC ${LIB_SRCS})
endif()
