# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

pkgconfig = import('pkgconfig')

zimg_include_dirs = include_directories('.')

libzimg_src = files(
  'api/zimg.cpp',
  'colorspace/colorspace.cpp',
  'colorspace/colorspace_param.cpp',
  'colorspace/gamma.cpp',
  'colorspace/graph.cpp',
  'colorspace/matrix3.cpp',
  'colorspace/operation.cpp',
  'colorspace/operation_impl.cpp',
  'common/cpuinfo.cpp',
  'common/libm_wrapper.cpp',
  'common/matrix.cpp',
  'depth/blue.cpp',
  'depth/depth_convert.cpp',
  'depth/depth.cpp',
  'depth/dither.cpp',
  'depth/quantize.cpp',
  'graph/basic_filter.cpp',
  'graph/filtergraph.cpp',
  'graph/graphbuilder.cpp',
  'graph/graphnode.cpp',
  'resize/filter.cpp',
  'resize/resize.cpp',
  'resize/resize_impl.cpp',
  'unresize/bilinear.cpp',
  'unresize/unresize.cpp',
  'unresize/unresize_impl.cpp',
)

libsse_src = files(
	'colorspace/x86/operation_impl_sse.cpp',
	'resize/x86/resize_impl_sse.cpp',
)

libsse2_src = files(
	'colorspace/x86/operation_impl_sse2.cpp',
	'depth/x86/depth_convert_sse2.cpp',
	'depth/x86/dither_sse2.cpp',
	'depth/x86/error_diffusion_sse2.cpp',
	'depth/x86/f16c_sse2.cpp',
	'resize/x86/resize_impl_sse2.cpp',
)
libavx_src = files(
	'colorspace/x86/operation_impl_avx.cpp',
	'resize/x86/resize_impl_avx.cpp',
)

libf16c_src = files('depth/x86/f16c_ivb.cpp')

libavx2_src = files(
  'colorspace/x86/operation_impl_avx2.cpp',
  'depth/x86/depth_convert_avx2.cpp',
  'depth/x86/dither_avx2.cpp',
  'depth/x86/error_diffusion_avx2.cpp',
  'resize/x86/resize_impl_avx2.cpp',
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
  'depth/arm/f16c_neon.cpp',
  'resize/arm/resize_impl_neon.cpp',
)

zimg_internal = []

if get_option('simd')
  if host_machine.cpu_family().contains('x86')
    libzimg_src += files(
      'colorspace/x86/operation_impl_x86.cpp',
      'common/x86/cpuinfo_x86.cpp',
      'common/x86/x86util.cpp',
      'depth/x86/depth_convert_x86.cpp',
      'depth/x86/dither_x86.cpp',
      'resize/x86/resize_impl_x86.cpp',
    )
    
    zimg_internal += [
      library('zimg_sse', libsse_src,
        cpp_args: sse_args,
        gnu_symbol_visibility: 'hidden',
        include_directories: zimg_include_dirs,
        build_by_default: false).extract_all_objects(recursive: true),
      library('zimg_sse2', libsse2_src,
        cpp_args: sse2_args,
        gnu_symbol_visibility: 'hidden',
        include_directories: zimg_include_dirs,
        build_by_default: false).extract_all_objects(recursive: true),
      library('zimg_avx', libavx_src,
        cpp_args: avx_args,
        gnu_symbol_visibility: 'hidden',
        include_directories: zimg_include_dirs,
        build_by_default: false).extract_all_objects(recursive: true),
      library('zimg_f16c', libf16c_src,
        cpp_args: f16c_args,
        gnu_symbol_visibility: 'hidden',
        include_directories: zimg_include_dirs,
        build_by_default: false).extract_all_objects(recursive: true),
      library('zimg_avx2', libavx2_src,
        cpp_args: avx2_args,
        gnu_symbol_visibility: 'hidden',
        include_directories: zimg_include_dirs,
        build_by_default: false).extract_all_objects(recursive: true),
    ]
    if have_avx512
      zimg_internal += [
        library('zimg_avx512', libavx512_src,
          cpp_args: avx512_args,
          gnu_symbol_visibility: 'hidden',
          include_directories: zimg_include_dirs,
          build_by_default: false).extract_all_objects(recursive: true),
        library('zimg_avx512vnni', libavx512vnni_src,
          cpp_args: avx512vnni_args,
          gnu_symbol_visibility: 'hidden',
          include_directories: zimg_include_dirs,
          build_by_default: false).extract_all_objects(recursive: true),
      ]
    endif

  elif host_machine.cpu_family() in ['arm', 'aarch64']
   
    libzimg_src += files(
      'colorspace/arm/operation_impl_arm.cpp',
      'common/arm/cpuinfo_arm.cpp',
      'common/arm/neon_util.cpp',
      'depth/arm/depth_convert_arm.cpp',
      'depth/arm/dither_arm.cpp',
      'resize/arm/resize_impl_arm.cpp',
    )
    zimg_internal += library('zimg_neon', libneon_src,
      cpp_args: neon_args,
      gnu_symbol_visibility: 'hidden',
      include_directories: zimg_include_dirs,
      build_by_default: false).extract_all_objects(recursive: true)

  endif
endif

zimg_internal += library('zimg_internal', libzimg_src,
  gnu_symbol_visibility: 'hidden',
  include_directories: zimg_include_dirs,
  build_by_default: false).extract_all_objects(recursive: true)

if windows_msvc
  zimg_name = 'z'
  zimg_version = {}
else
  zimg_name = 'zimg'
  zimg_version = { 'version': '2.0.0' }
endif

libzimg = library(zimg_name, 'api/zimg.h',
  objects: zimg_internal,
  dependencies: libm,
  gnu_symbol_visibility: 'hidden',
  kwargs: zimg_version,
  install: true)

zimg = declare_dependency(link_with: libzimg, include_directories: 'api')
meson.override_dependency('zimg', zimg)

if get_option('stdlib') != ''
  stdlib = '-l' + get_option('stdlib').strip('lib')
else
  if cpp.has_header_symbol('cstdlib', '__GLIBCXX__')
    stdlib = '-lstdc++'
  elif cpp.has_header_symbol('cstdlib', '_LIBCPP_VERSION')
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

pkgconfig.generate(libzimg,
  name: 'zimg', kwargs: stdlib_pc,
  description: 'Scaling, colorspace conversion, and dithering library')

install_headers('api/zimg.h', 'api/zimg++.hpp')
