project('cpp-pds', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++14'])

srclist = [
  'src/main.cpp',
  'deps/MurmurHash3.cpp',
  ]
exe = executable('cpp-pds', srclist,
  install : true)

test('basic', exe)
