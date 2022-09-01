project('file_socket', 'c', 'cpp',
  default_options: ['c_std=c11', 'cpp_std=c++11', 'default_library=static', 'warning_level=3', 'werror=true'])

incdir = include_directories('src')
compiler = meson.get_compiler('cpp')
ws2dep = compiler.find_library('ws2_32', required: false)
thread_dep = dependency('threads')

sources_serv = [
  'src/socket_windows.cpp',
  'src/socket_posix.cpp',
  'src/ipc.cpp',
  'tests/ipc_server.cpp'
]

sources_client = [
  'src/socket_windows.cpp',
  'src/socket_posix.cpp',
  'src/ipc.cpp',
  'tests/ipc_client.cpp'
]

sources_tests = [
  'src/socket_windows.cpp',
  'src/socket_posix.cpp',
  'src/ipc.cpp',
  'tests/ipc_tests.cpp'
]


executable('ipc_server',
  sources_serv,
  include_directories: incdir,
  dependencies: ws2dep)

executable('ipc_client',
  sources_client,
  include_directories: incdir,
  dependencies: ws2dep)

executable('ipc_tests',
  sources_tests,
  include_directories: incdir,
  dependencies: [ws2dep, thread_dep])

