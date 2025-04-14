# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

set(testapp_src
  colorspaceapp.cpp
  cpuinfoapp.cpp
  depthapp.cpp
  frame.cpp
  graphapp.cpp
  main.cpp
  resizeapp.cpp
  table.cpp
  unresizeapp.cpp
  utils.cpp
)
  
add_executable(testapp ${testapp_src})
target_link_libraries(testapp PRIVATE testcommon zimg::zimg zimg_internal graphengine)
target_include_directories(testapp PRIVATE ../testcommon)
