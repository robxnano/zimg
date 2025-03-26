# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

testcommon_inc_dirs = include_directories('../testcommon')

testapp_src = files(
  'colorspaceapp.cpp',
  'cpuinfoapp.cpp',
  'depthapp.cpp',
  'frame.cpp',
  'graphapp.cpp',
  'main.cpp',
  'resizeapp.cpp',
  'table.cpp',
  'unresizeapp.cpp',
  'utils.cpp',
)

executable('testapp', testapp_src,
  link_with: [libtestcommon, zimg_libs],
  include_directories: [inc_dirs, testcommon_inc_dirs],
  build_by_default: false)
