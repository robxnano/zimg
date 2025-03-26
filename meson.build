# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

project('zimg', 'c', 'cpp',
  version: '3.0.5',
  license: 'WTFPL',
  meson_version: '>= 0.55.0',
  default_options: ['buildtype=release',
                    'b_ndebug=if-release',
                    'c_std=c99',
                    'cpp_std=c++14',
                    'optimization=2',
                    'warning_level=2'])

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')
windows_msvc = cpp.get_argument_syntax() == 'msvc'

link_args = []
args = []

if not windows_msvc
  args += cpp.get_supported_arguments('-Wno-unused-parameter', '-Wno-missing-field-initializers')
endif

if get_option('debug')
  args += cpp.get_supported_arguments('-g3', '-ggdb')
endif

if get_option('simd')
  if host_machine.cpu_family().contains('x86')
    args += '-DZIMG_X86'
    have_avx512 = true
    if cpp.get_id() == 'msvc'
      sse_args = []
      sse2_args = []
      avx_args = []
      f16c_args = []
      avx2_args = '/arch:AVX2'
      avx512_args = '/arch:AVX512'
      avx512vnni_args = '/arch:AVX512'
    elif cpp.get_id() == 'clang-cl'
      sse_args = '/clang:-msse'
      sse2_args = '/clang:-msse2'
      avx_args = ['/clang:-mavx', '/clang:-mtune=sandybridge']
      f16c_args = ['/clang:-mavx', '/clang:-mf16c', '/clang:-mtune=ivybridge']
      avx2_args = ['/clang:-mavx2', '/clang:-mf16c', '/clang:-mfma', '/clang:-mtune=haswell']
      avx512_base = ['/clang:-mavx512f', '/clang:-mavx512cd', '/clang:-mavx512vl', '/clang:-mavx512bw', '/clang:-mavx512dq']
      avx512_args = avx512_base + '/clang:-mtune=skylake-avx512'
      avx512vnni_args = avx512_base + ['/clang:-mavx512vnni', '/clang:-mtune=cascadelake']
    else 
      sse_args = '-msse'
      sse2_args = '-msse2'
      avx_args = ['-mavx', '-mtune=sandybridge']
      f16c_args = ['-mavx', '-mf16c', '-mtune=ivybridge']
      avx2_args = ['-mavx2', '-mf16c', '-mfma', '-mtune=haswell']
      avx512_args = ['-mavx512f', '-mavx512cd', '-mavx512vl', '-mavx512bw', '-mavx512dq']
      avx512vnni_args = ['-mavx512vnni']
      if cpp.has_multi_arguments(avx512_args + avx512vnni_args)
        avx512_args += cpp.get_supported_arguments('-mtune=skylake-avx512')
        avx512vnni_args += avx512_args + cpp.get_supported_arguments('-mtune=cascadelake')
      else
        have_avx512 = false
      endif
    endif

    if have_avx512
      args += '-DZIMG_X86_AVX512'
    endif
  elif host_machine.cpu_family() == 'aarch64'
    args += '-DZIMG_ARM'
    neon_args = []
  elif host_machine.cpu_family() == 'arm'
    args += '-DZIMG_ARM'
    if cpp.get_id() == 'clang-cl'
      neon_args = ['/clang:-march=armv7-a', '/clang:-mfpu=neon-vfpv4']
    elif cpp.get_id() != 'msvc'
      neon_args = ['-march=armv7-a', '-mfpu=neon-vfpv4', cpp.get_supported_arguments('-mfp16-format=ieee')]
    else
      neon_args = []
    endif
  endif
endif

if host_machine.system() in ['windows', 'cygwin'] and (not windows_msvc) and cc.sizeof('void *') == 4
  add_project_link_arguments('-Wl,--kill-at', language: ['c', 'cpp'])
endif

if host_machine.system() == 'cygwin'
  args += '-D_POSIX_C_SOURCE=200809L'
endif

libm = cc.find_library('m', required: false)

add_project_arguments(args, language: ['c', 'cpp'])

subdir('src/zimg')
subdir('doc/example')

if get_option('testapp')
  subdir('src/testcommon')
  subdir('src/testapp')
endif

if get_option('unit_tests')
  subdir('test')
endif

install_data('ChangeLog', 'COPYING',
  install_dir: get_option('datadir') / 'doc/zimg')
