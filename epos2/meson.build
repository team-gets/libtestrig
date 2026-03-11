cc = meson.get_compiler('c')

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
    dependencies: cc.find_library('EposCmd',
    dirs: [meson.current_source_dir() / 'bin'],
    required: false),
    version: '6.8.1.0'
  ) 

  if (epos2.found())
    message('Controller capabilities being built.')
    epos2_srcs = [ './epos2/constants.c', './epos2/ident.c' ]
  else
    message('[WARNING] Controller capabilities not being built. Continuing to create library anyway')
    epos2_srcs = [ ]
  endif

# Others
else
  error('Unsupported operating system')
endif
