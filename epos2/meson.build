cc = meson.get_compiler('c')

epos2_srcs = [ './constants.c', './ident.c' ]

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
    required: false)) 
    #FIXME this is for CI

# Others
else
  error('Unsupported operating system')
endif
