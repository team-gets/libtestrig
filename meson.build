project('libtestrig', 'c')

srcs = [ 'lib/ipc.c' ]
lib = shared_library('testrig', srcs)
dep = declare_dependency(include_directories: 'lib', link_with: lib)
