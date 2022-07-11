project('file_socket','c', 'cpp', 
  default_options : ['c_std=c11' , 'cpp_std=c++11'] )

incdir = include_directories('src')

sources = [ 
  'src/ipc.cpp',
  'src/test_library.cpp'
]

executable('test_library', 
  sources,
  include_directories: incdir)

# sample code from docs
executable('example_socket_server', 'example/server.c')
executable('example_socket_client', 'example/client.c')

