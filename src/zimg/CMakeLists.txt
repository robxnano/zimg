# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

add_library(graphengine OBJECT
../../graphengine/graphengine/cpuinfo.cpp
../../graphengine/graphengine/graph.cpp
../../graphengine/graphengine/node.cpp
../../graphengine/graphengine/x86/cpuinfo_x86.cpp
)
target_include_directories(graphengine PUBLIC ${PROJECT_SOURCE_DIR}/graphengine/include)

add_library(zimg_internal OBJECT
  api/zimg.cpp
  colorspace/colorspace.cpp
  colorspace/colorspace_graph.cpp
  colorspace/colorspace_param.cpp
  colorspace/gamma.cpp
  colorspace/matrix3.cpp
  colorspace/operation.cpp
  colorspace/operation_impl.cpp
  common/cpuinfo.cpp
  common/libm_wrapper.cpp
  common/matrix.cpp
  depth/blue.cpp
  depth/depth.cpp
  depth/depth_convert.cpp
  depth/dither.cpp
  depth/quantize.cpp
  graph/filter_base.cpp
  graph/filtergraph.cpp
  graph/graphbuilder.cpp
  graph/graphengine_except.cpp
  graph/simple_filters.cpp
  resize/filter.cpp
  resize/resize.cpp
  resize/resize_impl.cpp
  unresize/bilinear.cpp
  unresize/unresize.cpp
  unresize/unresize_impl.cpp
)

set(zimg_avx2_src
  colorspace/x86/operation_impl_avx2.cpp
  depth/x86/depth_convert_avx2.cpp
  depth/x86/dither_avx2.cpp
  depth/x86/error_diffusion_avx2.cpp
  resize/x86/resize_impl_avx2.cpp
  unresize/x86/unresize_impl_avx2.cpp
)

set(zimg_avx512_src
  colorspace/x86/gamma_constants_avx512.cpp
  colorspace/x86/operation_impl_avx512.cpp
  depth/x86/depth_convert_avx512.cpp
  depth/x86/dither_avx512.cpp
  resize/x86/resize_impl_avx512.cpp
)

set(zimg_avx512vnni_src
  resize/x86/resize_impl_avx512_vnni.cpp
)

set(neon_src
  colorspace/arm/operation_impl_neon.cpp
  depth/arm/depth_convert_neon.cpp
  depth/arm/dither_neon.cpp
  resize/arm/resize_impl_neon.cpp
)

if(ZIMG_ENABLE_SIMD)
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64|x64)$")
    target_sources(zimg_internal PRIVATE
      colorspace/x86/operation_impl_x86.cpp
      common/x86/cpuinfo_x86.cpp
      common/x86/x86util.cpp
      depth/x86/depth_convert_x86.cpp
      depth/x86/dither_x86.cpp
      resize/x86/resize_impl_x86.cpp
      unresize/x86/unresize_impl_x86.cpp
      ${zimg_avx2_src}
      ${zimg_avx512_src}
      ${zimg_avx512vnni_src})
    if(MSVC)
      set_source_files_properties(${zimg_avx2_src} PROPERTIES COMPILE_OPTIONS /arch:AVX2)
      set_source_files_properties(${zimg_avx512_src} PROPERTIES COMPILE_OPTIONS /arch:AVX512)
      set_source_files_properties(${zimg_avx512vnni_src} PROPERTIES COMPILE_OPTIONS /arch:AVX512)
      if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set_source_files_properties(${zimg_avx512_src} PROPERTIES
            COMPILE_OPTIONS "/arch:AVX512;/clang:-mavx512f;/clang:-mavx512cd;/clang:-mavx512vl;/clang:-mavx512bw;/clang:-mavx512dq;/clang:-mtune=skylake-avx512")
        set_source_files_properties(${zimg_avx512vnni_src} PROPERTIES
            COMPILE_OPTIONS "/arch:AVX512;/clang:-mavx512f;/clang:-mavx512cd;/clang:-mavx512vl;/clang:-mavx512bw;/clang:-mavx512dq;/clang:-mavx512vnni;/clang:-mtune=cascadelake")
      endif()
    else()
      set_source_files_properties(${zimg_avx2_src} PROPERTIES
          COMPILE_OPTIONS "-mavx2;-mf16c;-mfma;-mtune=haswell")
      set_source_files_properties(${zimg_avx512_src} PROPERTIES
          COMPILE_OPTIONS "-mavx512f;-mavx512cd;-mavx512vl;-mavx512bw;-mavx512dq;-mtune=skylake-avx512")
      set_source_files_properties(${zimg_avx512vnni_src} PROPERTIES
          COMPILE_OPTIONS "-mavx512f;-mavx512cd;-mavx512vl;-mavx512bw;-mavx512dq;-mavx512vnni;-mtune=cascadelake")
    endif()
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)$")
    target_sources(zimg_internal PRIVATE
      colorspace/arm/operation_impl_arm.cpp
      common/arm/cpuinfo_arm.cpp
      common/arm/neon_util.cpp
      depth/arm/depth_convert_arm.cpp
      depth/arm/dither_arm.cpp
      resize/arm/resize_impl_arm.cpp
      ${neon_src}
    )
  endif()
endif()

target_link_libraries(zimg_internal PUBLIC graphengine)
target_include_directories(zimg_internal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/api)
if(ZIMG_GRAPHENGINE_API)
  target_compile_definitions(zimg_internal PRIVATE ZIMG_GRAPHENGINE_API)
endif()

if(MSVC)
  set(zimg_name z)
else()
  set(zimg_name zimg)
endif()

add_library(${zimg_name})
target_link_libraries(${zimg_name} PRIVATE zimg_internal graphengine)
target_include_directories(${zimg_name} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>")
set_target_properties(${zimg_name} PROPERTIES VERSION 2.0.0 SOVERSION 2)
set_target_properties(${zimg_name} PROPERTIES PUBLIC_HEADER "api/zimg++.hpp;api/zimg.h")

add_library(zimg::zimg ALIAS ${zimg_name})
    
install(TARGETS ${zimg_name}
  EXPORT zimg-targets
  RUNTIME LIBRARY PUBLIC_HEADER)
    