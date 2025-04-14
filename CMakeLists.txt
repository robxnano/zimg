# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.13...4.0)
project(zimg
  VERSION 3.99.0
  DESCRIPTION "Scaling, color space conversion, and dithering library"
  HOMEPAGE_URL https://github.com/sekrit-twc/zimg
  LANGUAGES C CXX
)

include(GNUInstallDirs)
include(CheckCXXSymbolExists)
include(CMakePackageConfigHelpers)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)

if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  if(MINGW)
    set(CMAKE_DLL_NAME_WITH_SOVERSION ON)
  endif()
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE
      STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS None Debug Release MinSizeRel RelWithDebInfo)
endif()

option(ZIMG_ENABLE_SIMD "Enable SIMD code" ON)
option(ZIMG_GRAPHENGINE_API "Enable the zimg graphengine API (experimental)" OFF)
option(ZIMG_TESTAPP "Compile the test application" OFF)
option(ZIMG_UNIT_TESTS "Compile unit tests (may result in slower code)" OFF)
if(NOT ZIMG_CXX_STDLIB)
  set(ZIMG_CXX_STDLIB "" CACHE STRING "C++ standard library for static linking (overrides automatic detection)")
endif()

add_compile_definitions(GRAPHENGINE_IMPL_NAMESPACE=zimg)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/graphengine/include/graphengine/graph.h)
  message(FATAL_ERROR "graphengine submodule is not available.\nUse 'git submodule update --init' to download it.")
endif()

if(MSVC)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
else()
  add_compile_options(-Wno-unused-parameter -Wno-missing-field-initializers)
  add_compile_options("$<$<CONFIG:Debug,RelWithDebInfo>:-g3;-ggdb>")
endif()

add_compile_definitions("$<$<CONFIG:Release,MinSizeRel>:NDEBUG>")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options("$<$<CONFIG:Debug,RelWithDebInfo>:-O1>")
endif()

if(ZIMG_ENABLE_SIMD)
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|x64|amd64|AMD64")
    add_compile_definitions(ZIMG_X86)
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    add_compile_definitions(ZIMG_ARM)
  endif()
endif()

if(WIN32 AND NOT MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
  add_link_options(-Wl,--kill-at)
endif()

if(ZIMG_CXX_STDLIB STREQUAL "" AND NOT MSVC)
  check_cxx_symbol_exists(__GLIBCXX__ cstddef HAVE_LIBSTDCXX)
  if(HAVE_LIBSTDCXX)
    set(ZIMG_CXX_STDLIB -lstdc++)
  else()
    check_cxx_symbol_exists(_LIBCPP_VERSION cstdlib HAVE_LIBCPP)
    if(HAVE_LIBCPP)
      set(ZIMG_CXX_STDLIB -lc++)
    else()
      message(WARNING "Unknown C++ standard library, please set ZIMG_CXX_STDLIB to ensure correct linkage.")
    endif()
  endif()
endif()

add_subdirectory(src/zimg)
add_subdirectory(doc/example)

if(ZIMG_TESTAPP)
  add_subdirectory(src/testcommon)
  add_subdirectory(src/testapp)
endif()

if(ZIMG_UNIT_TESTS)
  if(APPLE OR NOT CMAKE_CROSSCOMPILING OR CMAKE_CROSSCOMPILING_EMULATOR)
    add_subdirectory(test)
  else()
    message(WARNING "Cannot run tests when cross-compiling unless CMAKE_CROSSCOMPILING_EMULATOR is set.")
  endif()
endif()

install(FILES ChangeLog COPYING DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/zimg")

# Configure the pkg-config .pc file
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
set(libdir "\${prefix}/${CMAKE_INSTALL_LIBDIR}")
set(VERSION ${PROJECT_VERSION})
set(STL_LIBS ${ZIMG_CXX_STDLIB})

configure_file(zimg.pc.in zimg.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/zimg.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

if(NOT ZIMG_NAMESPACE)
  set(ZIMG_NAMESPACE "zimg" CACHE STRING "Namespace for CMake zimg targets")
endif()

configure_package_config_file(zimg-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/${ZIMG_NAMESPACE}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ZIMG_NAMESPACE}
  PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${ZIMG_NAMESPACE}-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)
install(EXPORT zimg-targets
        NAMESPACE ${ZIMG_NAMESPACE}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ZIMG_NAMESPACE})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${ZIMG_NAMESPACE}-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${ZIMG_NAMESPACE}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ZIMG_NAMESPACE}
)
