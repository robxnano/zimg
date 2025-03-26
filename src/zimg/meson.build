pkgconfig = import('pkgconfig')

inc_dirs = include_directories('.')

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
  'unresize/unresize_impl.cpp')

if enable_arm_simd
  libzimg_src += files(
    'colorspace/arm/operation_impl_arm.cpp',
    'common/arm/cpuinfo_arm.cpp',
    'common/arm/neon_util.cpp',
    'depth/arm/depth_convert_arm.cpp',
    'depth/arm/dither_arm.cpp',
    'resize/arm/resize_impl_arm.cpp')
elif enable_x86_simd
  libzimg_src += files(
    'colorspace/x86/operation_impl_x86.cpp',
    'common/x86/cpuinfo_x86.cpp',
    'common/x86/x86util.cpp',
    'depth/x86/depth_convert_x86.cpp',
    'depth/x86/dither_x86.cpp',
    'resize/x86/resize_impl_x86.cpp')
endif

libneon_src = files(
  'colorspace/arm/operation_impl_neon.cpp',
  'depth/arm/depth_convert_neon.cpp',
  'depth/arm/dither_neon.cpp',
  'depth/arm/f16c_neon.cpp',
  'resize/arm/resize_impl_neon.cpp')

libsse_src = files(
    'colorspace/x86/operation_impl_sse.cpp',
    'resize/x86/resize_impl_sse.cpp')

libsse2_src = files(
  'colorspace/x86/operation_impl_sse2.cpp',
  'depth/x86/depth_convert_sse2.cpp',
  'depth/x86/dither_sse2.cpp',
  'depth/x86/error_diffusion_sse2.cpp',
  'depth/x86/f16c_sse2.cpp',
  'resize/x86/resize_impl_sse2.cpp')

libavx_src = files(
  'colorspace/x86/operation_impl_avx.cpp',
  'resize/x86/resize_impl_avx.cpp')

libf16c_src = files(
  'depth/x86/f16c_ivb.cpp')

libavx2_src = files(
  'colorspace/x86/operation_impl_avx2.cpp',
  'depth/x86/depth_convert_avx2.cpp',
  'depth/x86/dither_avx2.cpp',
  'depth/x86/error_diffusion_avx2.cpp',
  'resize/x86/resize_impl_avx2.cpp')

libavx512_src = files(
  'colorspace/x86/gamma_constants_avx512.cpp',
  'colorspace/x86/operation_impl_avx512.cpp',
  'depth/x86/depth_convert_avx512.cpp',
  'depth/x86/dither_avx512.cpp',
  'resize/x86/resize_impl_avx512.cpp')

libavx512vnni_src = files(
  'resize/x86/resize_impl_avx512_vnni.cpp')

zimg_libs = []

if enable_x86_simd
  zimg_libs += [
    static_library('zimg_libsse', libsse_src,
      cpp_args: '-msse',
      include_directories: inc_dirs),
    static_library('zimg_libsse2', libsse2_src,
      cpp_args: '-msse2',
      include_directories: inc_dirs),
    static_library('zimg_libavx', libavx_src,
      cpp_args: '-mavx',
      include_directories: inc_dirs),
    static_library('zimg_libf16c', libf16c_src,
      cpp_args: ['-mavx', '-mf16c'],
      include_directories: inc_dirs),
    static_library('zimg_libavx2', libavx2_src,
      cpp_args: ['-mavx2', '-mf16c', '-mfma'],
      include_directories: inc_dirs)]
  if enable_avx512
    zimg_libs += [
      static_library('zimg_avx512', libavx512_src,
        cpp_args: ['-mavx512f', '-mavx512cd', '-mavx512vl', '-mavx512bw', '-mavx512dq'],
        include_directories: inc_dirs),
      static_library('zimg_avx512vnni', libavx512vnni_src,
        cpp_args: ['-mavx512f', '-mavx512cd', '-mavx512vl', '-mavx512bw', '-mavx512dq', '-mavx512vnni'],
        include_directories: inc_dirs)]
  endif
elif enable_arm_simd
  zimg_libs += static_library('zimg_neon', libneon_src,
    cpp_args: arm_args,
    include_directories: inc_dirs)
endif

zimg_libs += static_library('zimg_internal', libzimg_src,
  include_directories: inc_dirs)

libzimg = library('zimg', 'api/zimg.h',
  link_whole: zimg_libs,
  version: '2.0.0',
  install: true)

zimg = declare_dependency(link_with: libzimg, include_directories: 'api')

install_headers('api/zimg.h', 'api/zimg++.hpp')

if get_option('stdlib') != ''
  stdlib = '-l' + get_option('stdlib').strip('lib')
elif cpp.has_header_symbol('ciso646', '_LIBCPP_VERSION')
  stdlib = '-lc++'
else
  stdlib = '-lstdc++'
endif

pkgconfig.generate(libzimg,
  name: meson.project_name(),
  libraries_private: stdlib,
  description: 'Scaling, colorspace conversion, and dithering library')
