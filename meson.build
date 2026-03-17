project('libtestrig', 'c')

fs = import('fs')

cc = meson.get_compiler('c')

subdir('ipc')

srcs = [ 'ipc/ipc.c', 'ipc/message.c', 'ipc/constants.c', 'ipc/os.c' ]
lib = shared_library('testrig', srcs, dependencies: ipc_os_deps)
dep = declare_dependency(include_directories: '.', link_with: lib)

subdir('test')