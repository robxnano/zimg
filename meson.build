project('zimg', 'c', 'cpp',
  version: '3.0.5',
  meson_version: '>= 0.55.0',
  default_options: ['buildtype=release', 'c_std=c99', 'cpp_std=c++17', 'warning_level=2'])

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')
fs = import('fs')

link_args = []
args = ['-DGRAPHENGINE_IMPL_NAMESPACE=zimg']
enable_arm_simd = false
enable_x86_simd = false
enable_avx512 = false

if cpp.first_supported_argument('-std=c++17', '/std:c++17') == []
  error('C++ compiler must support at least C++17')
endif

if not fs.is_file('graphengine/include/graphengine/graph.h')
  error('graphengine submodule is not available.\nUse \'git submodule update --init\' to download it.')
endif

args += cpp.get_supported_arguments(
  '-fvisibility=hidden',
  '-Wno-unused-parameter',
  '-Wno-missing-field-initializers')

if get_option('debug')
  args += '-ggdb'
else
  args += '-DNDEBUG'
endif

avx512_test = '''
#include <immintrin.h>
int main()
{
  volatile __m512 x = _mm512_setzero_ps();
  x = _mm512_add_ps(x, x);
#if defined(__GNUC__) && defined(__x86_64__)
  asm volatile ("" : : : "zmm16", "zmm17", "zmm18", "zmm19");
#endif
  return 0;
}
'''

if get_option('simd')
  if host_machine.cpu_family() in ['x86', 'x86_64']
    enable_x86_simd = true
    args += '-DZIMG_X86'
    args += cpp.get_supported_arguments('-mtune=sandybridge', '-mtune=ivybridge',
      '-mtune=haswell', '-mtune=skylake-avx512', '-mtune=cascadelake')

    avx512_args = ['-mavx512f', '-mavx512pf', '-mavx512er',
      '-mavx512cd', '-mavx512vl', '-mavx512bw', '-mavx512dq', '-mavx512ifma',
      '-mavx512vbmi', '-mavx512vbmi2', '-mavx512bitalg', '-mavx512vpopcntdq', '-mavx512vnni']

    if cpp.compiles(avx512_test, name: 'avx512_test', args: ['-Werror', args, avx512_args])
      enable_avx512 = true
    endif
  elif host_machine.cpu_family() == 'aarch64'
    enable_arm_simd = true
    args += '-DZIMG_ARM'
    arm_args = []
  elif host_machine.cpu_family() == 'arm'
    enable_arm_simd = true
    args += '-DZIMG_ARM'
    arm_args = cpp.get_supported_arguments('-march=armv7-a', '-mfpu=neon-vfpv4', '-mfp16-format=ieee')
  endif
endif

if host_machine.system() in ['windows', 'cygwin']
  if cc.sizeof('void *') == 4 and cpp.get_id() not in ['msvc', 'clang-cl']
    add_project_link_arguments('-Wl,--kill-at', language: ['c', 'cpp'])
  endif
endif

add_project_arguments(args, language: ['c', 'cpp'])

subdir('src/zimg')

subdir('doc/example')

subdir('src/testcommon')
subdir('src/testapp')

if get_option('tests')
  subdir('test')
endif

install_data('ChangeLog', 'COPYING',
  install_dir: get_option('datadir') / 'doc/zimg')
