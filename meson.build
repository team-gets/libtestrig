project('libtestrig', 'c')

srcs = [ 'ipc/ipc.c', 'ipc/constants.c' ]
lib = shared_library('testrig', srcs)
dep = declare_dependency(include_directories: '.', link_with: lib)
