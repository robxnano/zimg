# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

testcommon_src = files(
  'argparse.cpp',
  'json.cpp',
  'mmap.cpp',
  'win32_bitmap.cpp')

libtestcommon = static_library('testcommon', testcommon_src,
  objects: zimg_internal,
  include_directories: zimg_include_dirs)

testcommon = declare_dependency(link_with: libtestcommon, include_directories: [zimg_include_dirs, '.'])