unit_test_src = files(
  '../graphengine/filter_validation/filter_validation.cpp',
  '../graphengine/filter_validation/sha1/sha1.c',
  'main.cpp',
  'api/api_test.cpp',
  'colorspace/colorspace_test.cpp',
  'colorspace/gamma_test.cpp',
  'depth/depth_convert_test.cpp',
  'depth/dither_test.cpp',
  'graph/graphbuilder_test.cpp',
  'resize/filter_test.cpp',
  'resize/resize_impl_test.cpp',
)

if get_option('simd')
  if host_machine.cpu_family() in ['arm', 'aarch64']
    unit_test_src += files(
      'colorspace/arm/colorspace_neon_test.cpp',
      'depth/arm/depth_convert_neon_test.cpp',
      'depth/arm/dither_neon_test.cpp',
      'depth/arm/f16c_neon_test.cpp',
      'resize/arm/resize_impl_neon_test.cpp',
  )
  elif host_machine.cpu_family() in ['x86', 'x86_64']
    unit_test_src += files(
      'colorspace/x86/colorspace_avx2_test.cpp',
      'depth/x86/depth_convert_avx2_test.cpp',
      'depth/x86/dither_avx2_test.cpp',
      'depth/x86/error_diffusion_avx2_test.cpp',
    )
  endif
  if enable_avx512
    unit_test_src += files(
      'colorspace/x86/colorspace_avx512_test.cpp',
      'colorspace/x86/gamma_constants_avx512_test.cpp',
      'depth/x86/depth_convert_avx512_test.cpp',
      'depth/x86/dither_avx512_test.cpp',
      'resize/x86/resize_impl_avx512_test.cpp',
      'resize/x86/resize_impl_avx512_vnni_test.cpp',
    )
  endif
endif

libm_src = files(
  'extra/musl-libm/__cos.c',
  'extra/musl-libm/__math_divzerof.c',
  'extra/musl-libm/__math_invalidf.c',
  'extra/musl-libm/__math_oflowf.c',
  'extra/musl-libm/__math_uflowf.c',
  'extra/musl-libm/__math_xflowf.c',
  'extra/musl-libm/__rem_pio2.c',
  'extra/musl-libm/__rem_pio2_large.c',
  'extra/musl-libm/__sin.c',
  'extra/musl-libm/cos.c',
  'extra/musl-libm/exp2f_data.c',
  'extra/musl-libm/expf.c',
  'extra/musl-libm/fpu_wrapper.c',
  'extra/musl-libm/log10f.c',
  'extra/musl-libm/logf.c',
  'extra/musl-libm/logf_data.c',
  'extra/musl-libm/powf.c',
  'extra/musl-libm/powf_data.c',
  'extra/musl-libm/sin.c',
)

libm_args = cc.get_supported_arguments('-Wno-sign-compare', '-Wno-unused-variable', '-Wno-unused-but-set-variable')

libm = static_library('m_musl', libm_src,
  c_args: libm_args,
  build_by_default: false)

gtest = dependency('gtest')

unit_test_inc_dirs = include_directories('extra')

unit_test = executable('unit_test', unit_test_src,
  dependencies: gtest,
  link_with: [zimg_libs, libm],
  include_directories: [inc_dirs, unit_test_inc_dirs],
  build_by_default: false)

tests = [
  'APITest',
  'ColorspaceConversionTest',
  'DepthConvertTest',
  'DitherTest',
  'FilterGraphTest',
  'GammaTest',
  'GraphBuilderTest',
  'ResizeImplTest',
]
foreach t: tests
  test(t, unit_test, args: ['--gtest_filter=' + t + '.*'], protocol: 'gtest')
endforeach
