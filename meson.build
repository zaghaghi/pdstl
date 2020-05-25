project('pdstl', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++14'])

srclist = [
  'src/main.cpp',
  'deps/MurmurHash3.cpp',
  ]

incdir = include_directories('include')
depdir = include_directories('deps')

exe = executable('pdstl', srclist,
  install : true,
  include_directories : [incdir, depdir])

test('basic', exe)
