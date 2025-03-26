libtestcommon_src = files(
  'argparse.cpp',
  'json.cpp',
  'mmap.cpp',
  'win32_bitmap.cpp')

libtestcommon = static_library('testcommon', libtestcommon_src,
  link_with: zimg_libs,
  include_directories: inc_dirs)
