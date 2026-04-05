project('libtestrig', 'c', default_options: ['warning_level=3'])

fs = import('fs')
cc = meson.get_compiler('c')

## Machine info
operating_system = host_machine.system()
cpu_architecture = host_machine.cpu_family()
valid_os = ['windows', 'linux']
valid_arch = ['x86', 'x86_64', 'aarch64', 'arm']

subdir('lib')
subdir('test')
