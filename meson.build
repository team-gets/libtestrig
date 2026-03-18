project('libtestrig', 'c', default_options: ['warning_level=everything'])

fs = import('fs')
cc = meson.get_compiler('c')

## Machine info
operating_system = host_machine.system()
cpu_architecture = host_machine.cpu_family()
valid_os = ['windows', 'linux']
valid_arch = ['x86', 'x86_64', 'aarch64', 'arm']

subdir('ipc')

## Main Binary Definitions
subdir('epos2')

srcs = [ 'ipc/ipc.c', 'ipc/message.c', 'ipc/constants.c', 'ipc/os.c' ] + epos2_srcs
lib = shared_library('testrig', srcs, dependencies: ipc_os_deps, c_args: ['-DCOMPILING_TESTRIG_DLL'], dependencies: epos2 )
dep = declare_dependency(include_directories: '.',
  link_with: lib, dependencies: epos2)

subdir('test')