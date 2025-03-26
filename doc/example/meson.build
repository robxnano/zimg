# Copyright (C) 2025 Rob Hall
# SPDX-License-Identifier: MIT

misc_src = files(
  'misc/argparse.cpp',
  'misc/mmap.cpp',
  'misc/win32_bitmap.cpp',
)

examples = []
examples_install_dir = get_option('datadir') / 'doc/zimg/example'

examples += executable('api_example', ['api_example.cpp', misc_src],
  dependencies: zimg,
  include_directories: 'misc',
  build_by_default: false)

examples += executable('api_example_c', ['api_example_c.c', misc_src],
  dependencies: zimg,
  include_directories: 'misc',
  build_by_default: false)

examples += executable('graphengine_example', ['graphengine_example.cpp', misc_src, graphengine_src],
  dependencies: zimg,
  include_directories: 'misc',
  build_by_default: false)

examples += executable('hdr_example', ['hdr_example.cpp', misc_src],
  dependencies: zimg,
  include_directories: 'misc',
  build_by_default: false)

examples += executable('interlace_example', ['interlace_example.cpp', misc_src],
  dependencies: zimg,
  include_directories: 'misc',
  build_by_default: false)

examples += executable('tile_example', ['tile_example.cpp', misc_src],
  dependencies: zimg,
  include_directories: 'misc',
  build_by_default: false)

alias_target('examples', examples)

install_data(
  'Makefile',
  'api_example.cpp',
  'api_example_c.c',
  'hdr_example.cpp',
  'interlace_example.cpp',
  'tile_example.cpp',
  install_dir: examples_install_dir)

if meson.version().version_compare('>= 1.3.0')
  install_subdir('misc', install_dir: examples_install_dir, follow_symlinks: true)
else
  install_subdir('misc', install_dir: examples_install_dir)
endif
