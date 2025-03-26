# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

project('zimg', 'c', 'cpp',
  version: '3.0.5',
  meson_version: '>= 0.55.0',
  default_options: ['buildtype=release', 'b_ndebug=if-release',
                    'c_std=c99', 'cpp_std=c++17', 'warning_level=2'])

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')
fs = import('fs')
windows_msvc = host_machine.system() == 'windows' and cpp.get_argument_syntax() == 'msvc'

link_args = []
args = ['-DGRAPHENGINE_IMPL_NAMESPACE=zimg']

if not cpp.has_argument(windows_msvc ? '/std:c++17' : '-std=c++17')
  error('C++ compiler must support at least C++17')
endif

if not fs.is_file('graphengine/include/graphengine/graph.h')
  error('graphengine submodule is not available.\nUse \'git submodule update --init\' to download it.')
endif

if not windows_msvc
  args += cpp.get_supported_arguments(
    '-Wno-unused-parameter',
    '-Wno-missing-field-initializers')
endif

if get_option('debug')
  args += cpp.get_supported_arguments('-g3', '-ggdb')
endif

if get_option('simd')
  if host_machine.cpu_family() == 'x86_64'
    args += '-DZIMG_X86'
  elif host_machine.cpu_family() == 'aarch64'
    args += '-DZIMG_ARM'
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
subdir('src/testcommon')
subdir('src/testapp')
subdir('doc/example')

if get_option('tests')
  subdir('test')
endif

install_data('ChangeLog', 'COPYING',
  install_dir: get_option('datadir') / 'doc/zimg')
