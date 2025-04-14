# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

set(testcommon_src
  argparse.cpp
  json.cpp
  mmap.cpp
  win32_bitmap.cpp
)

add_library(testcommon STATIC ${testcommon_src})
target_link_libraries(testcommon PUBLIC zimg::zimg)
