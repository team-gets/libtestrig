fs = import('fs')
cc = meson.get_compiler('c')

# Windows
if (operating_system == 'windows')
  if not (cpu_architecture in ['x86', 'x86_64'])
    error('Unsupported architecture')
  endif

  where_epos2 = where_bin / 'EposCmd64.so'
  epos2 = declare_dependency(
    link_args: [ where_epos2 ],
    version: '6.8.1.0'
  )

# Linux
elif (operating_system == 'linux')
  if not (cpu_architecture in valid_arch)
    error('Unsupported architecture')
  endif

  where_ftd2xx = where_bin / 'libftd2xx.so'
  where_epos2 = where_bin / 'libEposCmd.so'

  ftd2xx = declare_dependency(
    link_args: [ where_ftd2xx ]
  )

  epos2 = declare_dependency(
    dependencies: ftd2xx,
    link_args: [ where_epos2 ],
    version: '6.8.1.0'
  ) 

# Others
else
  error('Unsupported operating system')
endif

epos2_srcs = [ './epos2/epos2.c', './epos2/definitions.c',
    './epos2/identify.c', './epos2/connect.c', './epos2/manage.c' ]

