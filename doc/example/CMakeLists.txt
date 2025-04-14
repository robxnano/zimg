# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

add_library(example_misc OBJECT
  misc/argparse.cpp
  misc/mmap.cpp
  misc/win32_bitmap.cpp
)
target_include_directories(example_misc PUBLIC misc)

set(EXAMPLE_DIR ${CMAKE_INSTALL_DATADIR}/doc/zimg/example)

add_executable(api_example api_example.cpp)
target_link_libraries(api_example PRIVATE zimg::zimg example_misc)

add_executable(api_example_c api_example_c.c)
target_link_libraries(api_example_c PRIVATE zimg::zimg example_misc)

add_executable(hdr_example hdr_example.cpp)
target_link_libraries(hdr_example PRIVATE zimg::zimg example_misc)

add_executable(interlace_example interlace_example.cpp)
target_link_libraries(interlace_example PRIVATE zimg::zimg example_misc)

add_executable(tile_example tile_example.cpp)
target_link_libraries(tile_example PRIVATE zimg::zimg example_misc)

if(ZIMG_GRAPHENGINE_API)
  add_executable(graphengine_example graphengine_example.cpp)
  target_link_libraries(graphengine_example PRIVATE zimg::zimg example_misc graphengine)
endif()

install(FILES
  Makefile
  api_example.cpp
  api_example_c.c
  hdr_example.cpp
  interlace_example.cpp
  tile_example.cpp
  DESTINATION ${EXAMPLE_DIR})

install(FILES
  ../../src/testcommon/aligned_malloc.h
  ../../src/testcommon/argparse.cpp
  ../../src/testcommon/argparse.h
  ../../src/testcommon/mmap.cpp
  ../../src/testcommon/mmap.h
  ../../src/testcommon/win32_bitmap.cpp
  ../../src/testcommon/win32_bitmap.h
  DESTINATION ${EXAMPLE_DIR}/misc
)
