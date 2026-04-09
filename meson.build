project('libtestrig', 'c', default_options: ['warning_level=3'])

fs = import('fs')
cc = meson.get_compiler('c')
pyexe = find_program('python')

## Machine info
operating_system = host_machine.system()
cpu_architecture = host_machine.cpu_family()
valid_os = ['windows', 'linux']
valid_arch = ['x86', 'x86_64', 'aarch64', 'arm']

where_bin = meson.project_source_root() / 'bin'
std_rpath = '$ORIGIN'

subdir('lib')
subdir('src')
subdir('test')
