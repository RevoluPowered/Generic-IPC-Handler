project('file_socket','c', 'cpp', 
  default_options : ['c_std=c11' , 'cpp_std=c++11'] )

incdir = include_directories('src')

sources_serv = [
  'src/socket_posix.cpp',
  'src/ipc.cpp',
  'src/ipc_server.cpp'
]

sources_client = [
  'src/socket_posix.cpp',
  'src/ipc.cpp',
  'src/ipc_client.cpp'
]

executable('ipc_server', 
  sources_serv,
  include_directories: incdir)


executable('ipc_client', 
  sources_client,
  include_directories: incdir)


# sample code from docs
executable('example_socket_server', 'example/server.c')
executable('example_socket_client', 'example/client.c')

