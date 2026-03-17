if (operating_system.to_lower() == 'linux')
    fork_exe = executable('test_exe', 'test_fork_ipc.c',
        dependencies: dep)
    preset_path_exe = executable('preset_path_exe', 'test_preset_sockpath.c',
        dependencies: dep)

    test('forking', fork_exe)
    test('preset_path', preset_path_exe)
elif (operating_system.to_lower() == 'windows')
    kernel32_dep = declare_dependency(
        dependencies: cc.find_library('Kernel32',
        required: true),
    ) 

    client_exe = executable('client_exe', 'test_win_client.c',
        dependencies: [ dep, kernel32_dep ],
        install_dir: meson.project_build_root() / '..')
    server_exe = executable('server_exe', 'test_win_server.c',
        dependencies: [ dep, kernel32_dep ],
        install_dir: meson.project_build_root() / '..')
endif