# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

pkgconfig = import('pkgconfig')

inc_dirs = include_directories('.', '../../graphengine/include')

graphengine_src = files(
  '../../graphengine/graphengine/cpuinfo.cpp',
  '../../graphengine/graphengine/graph.cpp',
  '../../graphengine/graphengine/node.cpp',
  '../../graphengine/graphengine/x86/cpuinfo_x86.cpp',
)

libzimg_src = graphengine_src + files(
  'api/zimg.cpp',
  'colorspace/colorspace.cpp',
  'colorspace/colorspace_graph.cpp',
  'colorspace/colorspace_param.cpp',
  'colorspace/gamma.cpp',
  'colorspace/matrix3.cpp',
  'colorspace/operation.cpp',
  'colorspace/operation_impl.cpp',
  'common/cpuinfo.cpp',
  'common/libm_wrapper.cpp',
  'common/matrix.cpp',
  'depth/blue.cpp',
  'depth/depth.cpp',
  'depth/depth_convert.cpp',
  'depth/dither.cpp',
  'depth/quantize.cpp',
  'graph/filter_base.cpp',
  'graph/filtergraph.cpp',
  'graph/graphbuilder.cpp',
  'graph/graphengine_except.cpp',
  'graph/simple_filters.cpp',
  'resize/filter.cpp',
  'resize/resize.cpp',
  'resize/resize_impl.cpp',
  'unresize/bilinear.cpp',
  'unresize/unresize.cpp',
  'unresize/unresize_impl.cpp',
)

libavx2_src = files(
  'colorspace/x86/operation_impl_avx2.cpp',
  'depth/x86/depth_convert_avx2.cpp',
  'depth/x86/dither_avx2.cpp',
  'depth/x86/error_diffusion_avx2.cpp',
  'resize/x86/resize_impl_avx2.cpp',
  'unresize/x86/unresize_impl_avx2.cpp',
)

libavx512_src = files(
  'colorspace/x86/gamma_constants_avx512.cpp',
  'colorspace/x86/operation_impl_avx512.cpp',
  'depth/x86/depth_convert_avx512.cpp',
  'depth/x86/dither_avx512.cpp',
  'resize/x86/resize_impl_avx512.cpp',
)

libavx512vnni_src = files(
  'resize/x86/resize_impl_avx512_vnni.cpp',
)

libneon_src = files(
  'colorspace/arm/operation_impl_neon.cpp',
  'depth/arm/depth_convert_neon.cpp',
  'depth/arm/dither_neon.cpp',
  'resize/arm/resize_impl_neon.cpp',
)

zimg_libs = []

if get_option('simd')
  if host_machine.cpu_family() == 'x86_64'

    libzimg_src += files(
      'colorspace/x86/operation_impl_x86.cpp',
      'common/x86/cpuinfo_x86.cpp',
      'common/x86/x86util.cpp',
      'depth/x86/depth_convert_x86.cpp',
      'depth/x86/dither_x86.cpp',
      'resize/x86/resize_impl_x86.cpp',
      'unresize/x86/unresize_impl_x86.cpp',
    )
    if windows_msvc
      avx2_args = '/arch:AVX2'
      avx512_args = '/arch:AVX512'
      avx512vnni_args = ['/arch:AVX512', cpp.get_supported_arguments('/clang:-mavx512vnni', '/clang:-mtune=cascadelake')]
    else
      avx2_args = ['-mavx2', '-mf16c', '-mfma',
        cpp.get_supported_arguments('-mtune=haswell')]
      avx512_args = ['-mavx512f', '-mavx512cd', '-mavx512vl', '-mavx512bw', '-mavx512dq',
        cpp.get_supported_arguments('-mtune=skylake-avx512')]
      avx512vnni_args = ['-mavx512f', '-mavx512cd', '-mavx512vl', '-mavx512bw', '-mavx512dq', '-mavx512vnni',
        cpp.get_supported_arguments('-mtune=cascadelake')]
    endif
    zimg_libs += [
      static_library('zimg_libavx2', libavx2_src,
        cpp_args: avx2_args,
        gnu_symbol_visibility: 'hidden',
        include_directories: inc_dirs),
      static_library('zimg_avx512', libavx512_src,
        cpp_args: avx512_args,
        gnu_symbol_visibility: 'hidden',
        include_directories: inc_dirs),
      static_library('zimg_avx512vnni', libavx512vnni_src,
        cpp_args: avx512vnni_args,
        gnu_symbol_visibility: 'hidden',
        include_directories: inc_dirs),
    ]

  elif host_machine.cpu_family() == 'aarch64'

    libzimg_src += files(
      'colorspace/arm/operation_impl_arm.cpp',
      'common/arm/cpuinfo_arm.cpp',
      'common/arm/neon_util.cpp',
      'depth/arm/depth_convert_arm.cpp',
      'depth/arm/dither_arm.cpp',
      'resize/arm/resize_impl_arm.cpp',
    )
    zimg_libs += static_library('zimg_neon', libneon_src,
      gnu_symbol_visibility: 'hidden',
      include_directories: inc_dirs)

  endif
endif

zimg_libs += static_library('zimg_internal', libzimg_src,
  cpp_args: '-DZIMG_GRAPHENGINE_API',
  gnu_symbol_visibility: 'hidden',
  include_directories: inc_dirs)

if windows_msvc
  zimg_name = 'z'
  zimg_version = {}
else
  zimg_name = 'zimg'
  zimg_version = { 'version': '2.0.0' }
endif

if get_option('stdlib') != ''
  stdlib = '-l' + get_option('stdlib').strip('lib')
else
  version_header = cpp.has_header('version') ? 'version' : 'ciso646'
  if cpp.has_header_symbol(version_header, '__GLIBCXX__')
    stdlib = '-lstdc++'
  elif cpp.has_header_symbol(version_header, '_LIBCPP_VERSION')
    stdlib = '-lc++'
  else
    stdlib = []
    warning('Unknown C++ standard library, please use -Dstdlib=<name> to ensure correct linkage')
  endif
endif

if get_option('default_library') == 'static'
  stdlib_pc = { 'libraries': stdlib }
else
  stdlib_pc = { 'libraries_private': stdlib }
endif

libzimg = library(zimg_name, 'api/zimg.h',
  link_whole: zimg_libs,
  dependencies: libm,
  gnu_symbol_visibility: 'hidden',
  vs_module_defs: '../../_msvc/zimg.def',
  kwargs: zimg_version,
  install: true)

zimg = declare_dependency(link_with: libzimg, include_directories: ['api', '../../graphengine/include'])
meson.override_dependency('zimg', zimg)

zimg_api_headers = files(
  'api/zimg.h',
  'api/zimg++.hpp',
)
graphengine_api_headers = files(
  '../../graphengine/include/graphengine/filter.h',
  '../../graphengine/include/graphengine/filter_validation.h',
  '../../graphengine/include/graphengine/graph.h',
  '../../graphengine/include/graphengine/namespace.h',
  '../../graphengine/include/graphengine/types.h',
)

install_headers(zimg_api_headers)
install_headers(graphengine_api_headers, subdir: 'graphengine')

pkgconfig.generate(libzimg,
  name: 'zimg', kwargs: stdlib_pc,
  description: 'Scaling, colorspace conversion, and dithering library')
