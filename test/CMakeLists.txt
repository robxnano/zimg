# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

include(GoogleTest)
add_subdirectory(extra/googletest EXCLUDE_FROM_ALL)
enable_testing()

set(unit_test_src
  ../graphengine/filter_validation/filter_validation.cpp
  ../graphengine/filter_validation/sha1/sha1.c
  main.cpp
  api/api_test.cpp
  colorspace/colorspace_test.cpp
  colorspace/gamma_test.cpp
  depth/depth_convert_test.cpp
  depth/dither_test.cpp
  graph/graphbuilder_test.cpp
  resize/filter_test.cpp
  resize/resize_impl_test.cpp
)

set(graphengine_unit_test_src
  ../graphengine/test/graph_node_test.cpp
  ../graphengine/test/graph_test.cpp
  ../graphengine/test/main.cpp
  ../graphengine/test/validator.cpp
)

if(ZIMG_ENABLE_SIMD)
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    list(APPEND unit_test_src
      colorspace/x86/colorspace_avx2_test.cpp
      colorspace/x86/colorspace_avx512_test.cpp
      colorspace/x86/gamma_constants_avx512_test.cpp
      depth/x86/depth_convert_avx2_test.cpp
      depth/x86/depth_convert_avx512_test.cpp
      depth/x86/dither_avx512_test.cpp
      depth/x86/dither_avx2_test.cpp
      depth/x86/error_diffusion_avx2_test.cpp
      resize/x86/resize_impl_avx512_test.cpp
      resize/x86/resize_impl_avx512_vnni_test.cpp
    )
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)$")
    list(APPEND unit_test_src
      colorspace/arm/colorspace_neon_test.cpp
      depth/arm/depth_convert_neon_test.cpp
      depth/arm/dither_neon_test.cpp
      depth/arm/f16c_neon_test.cpp
      resize/arm/resize_impl_neon_test.cpp
    )
  endif()
endif()

set(libm_src
  extra/musl-libm/__cos.c
  extra/musl-libm/__math_divzerof.c
  extra/musl-libm/__math_invalidf.c
  extra/musl-libm/__math_oflowf.c
  extra/musl-libm/__math_uflowf.c
  extra/musl-libm/__math_xflowf.c
  extra/musl-libm/__rem_pio2.c
  extra/musl-libm/__rem_pio2_large.c
  extra/musl-libm/__sin.c
  extra/musl-libm/cos.c
  extra/musl-libm/exp2f_data.c
  extra/musl-libm/expf.c
  extra/musl-libm/fpu_wrapper.c
  extra/musl-libm/log10f.c
  extra/musl-libm/logf.c
  extra/musl-libm/logf_data.c
  extra/musl-libm/powf.c
  extra/musl-libm/powf_data.c
  extra/musl-libm/sin.c
)

add_library(m_musl STATIC ${libm_src})
target_compile_options(m_musl PRIVATE "-Wno-sign-compare;-Wno-unused-variable;-Wno-unused-but-set-variable")

add_executable(unit_test ${unit_test_src})
target_link_libraries(unit_test PRIVATE zimg::zimg zimg_internal graphengine m_musl GTest::gtest)
target_include_directories(unit_test PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR}/extra)

if(WIN32) # TODO: Fix graphengine tests on Linux
  add_executable(graphengine_unit_test ${graphengine_unit_test_src})
  target_link_libraries(graphengine_unit_test PRIVATE zimg_internal graphengine m_musl GTest::gtest)
  target_include_directories(graphengine_unit_test PRIVATE
                             ${CMAKE_CURRENT_SOURCE_DIR}/extra
                             ${PROJECT_SOURCE_DIR}/graphengine/testapp)
endif()

gtest_discover_tests(unit_test)
