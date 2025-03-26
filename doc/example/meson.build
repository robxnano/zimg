misc_inc_dir = include_directories('misc')
misc_src = files(
  'misc/argparse.cpp',
  'misc/mmap.cpp',
  'misc/win32_bitmap.cpp',
)

api_example = executable('api_example', 'api_example.cpp', misc_src,
  dependencies: zimg,
  include_directories: misc_inc_dir,
)

api_example_c = executable('api_example_c', 'api_example_c.c', misc_src,
  dependencies: zimg,
  include_directories: misc_inc_dir,
)

hdr_example = executable('hdr_example', 'hdr_example.cpp', misc_src,
  dependencies: zimg,
  include_directories: misc_inc_dir,
)

interlace_example = executable('interlace_example', 'interlace_example.cpp', misc_src,
  dependencies: zimg,
  include_directories: misc_inc_dir,
)

tile_example = executable('tile_example', 'tile_example.cpp', misc_src,
  dependencies: zimg,
  include_directories: misc_inc_dir,
)

install_data(
  'Makefile',
  'api_example.cpp',
  'api_example_c.c',
  'hdr_example.cpp',
  'interlace_example.cpp',
  'tile_example.cpp',
  install_dir: get_option('datadir') / 'doc/zimg/example')

install_subdir('misc', install_dir: get_option('datadir') / 'doc/zimg/example')
