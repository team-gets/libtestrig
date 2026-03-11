# RM ME WHEN EPOS2 is merged
## Machine info
operating_system = host_machine.system()
cpu_architecture = host_machine.cpu_family()
valid_os = ['windows', 'linux']
valid_arch = ['x86', 'x86_64', 'aarch64', 'arm']

# END RM

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