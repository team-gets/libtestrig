## Main Binary Definitions
subdir('ipc')
subdir('epos2')

srcs = [ 'ipc/ipc.c', 'ipc/message.c', 'ipc/constants.c', 'ipc/os.c' ] + epos2_srcs
lib = shared_library('testrig', srcs,
  install_dir: where_bin,
  install_rpath: std_rpath,
  install: true,
  dependencies: [ ipc_os_deps, epos2 ],
  c_args: ['-DCOMPILING_TESTRIG_DLL'] )

dep = declare_dependency(include_directories: '.',
  link_with: lib, dependencies: epos2)
