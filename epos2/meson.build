fs = import('fs')
cc = meson.get_compiler('c')

if (fs.is_dir(meson.current_source_dir() / 'bin'))

message('Controller capabilities being built.')

# Windows
if (operating_system == 'windows')
  if not (cpu_architecture in ['x86', 'x86_64'])
    error('Unsupported architecture')
  endif

# Linux
elif (operating_system == 'linux')
  if not (cpu_architecture in valid_arch)
    error('Unsupported architecture')
  endif

  epos2 = declare_dependency(
    version: '6.8.1.0',
    dependencies: cc.find_library('EposCmd',
      dirs: [meson.current_source_dir() / 'bin'],
      required: false),
  ) 

  epos2_srcs = [ './epos2/constants.c', './epos2/epos2.c' ]

# Others
else
  error('Unsupported operating system')
endif

# CI etc.
else
  message('[WARNING] Controller capabilities not being built. Continuing to create library anyway')
  epos2 = declare_dependency()
  epos2_srcs = [ ]
endif
