fs = import('fs')
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

  ftd2xx = declare_dependency(
    link_args: meson.project_source_root() / 'bin' / 'libftd2xx.so.1.4.8'
  )

  epos2 = declare_dependency(
    dependencies: ftd2xx,
    link_args: meson.project_source_root() / 'bin' / 'libEposCmd.so.6.8.1.0'
  ) 

  epos2_srcs = [ './epos2/epos2.c', './epos2/definitions.c',
      './epos2/identify.c', './epos2/connect.c', './epos2/manage.c' ]

# Others
else
  error('Unsupported operating system')
endif
