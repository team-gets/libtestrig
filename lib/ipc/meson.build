if (operating_system.to_lower() == 'windows')
    ipc_os_deps = declare_dependency(
        dependencies: cc.find_library('Ws2_32',
        required: true),
  ) 
elif (operating_system.to_lower() == 'linux')
    ipc_os_deps = declare_dependency()
else
    error('Unsupported operating system or build environment: ' + operating_system)
endif
