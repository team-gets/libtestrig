project('libtestrig', 'c')

## Machine info
operating_system = host_machine.system()
cpu_architecture = host_machine.cpu_family()
valid_os = ['windows', 'linux']
valid_arch = ['x86', 'x86_64', 'aarch64', 'arm']

## Main Binary Definitions
subdir('epos2')
srcs = [ 'ipc/ipc.c', 'ipc/message.c', 'ipc/constants.c' ]
lib = shared_library('testrig', srcs)
dep = declare_dependency(include_directories: '.',
  link_with: lib, dependencies: epos2)

subdir('test')
