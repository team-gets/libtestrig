sock_name_exe = executable('test_sock_name', 'test_sock_name.c', dependencies: dep)
test('sockname', sock_name_exe)

if (operating_system.to_lower() == 'linux')
    fork_exe = executable('test_exe', 'test_fork_ipc.c',
        dependencies: dep,
        install_dir: where_bin,
        install_rpath: std_rpath,
        install: true)
    preset_path_exe = executable('preset_path_exe', 'test_preset_sockpath.c',
        dependencies: dep,
        install_dir: where_bin,
        install_rpath: std_rpath,
        install: true)

    test('forking', fork_exe)
    test('preset_path', preset_path_exe)

elif (operating_system.to_lower() == 'windows')
    kernel32_dep = declare_dependency(
        dependencies: cc.find_library('Kernel32',
        required: true),
    ) 
    
    dllcopy = [find_program('python'),
        meson.project_source_root() / 'test' / 'copy_dlls.meson.py',
        lib.full_path(),
        meson.project_source_root() / 'build' / 'test']

    mgr_exe = executable('test_mgr', 'test_win_mgr.c')
    client_exe = executable('test_client', 'test_win_client.c',
            dependencies: [ dep, kernel32_dep ])
    server_exe = executable('test_server', 'test_win_server.c',
            dependencies: [ dep, kernel32_dep ])

    move_dlls_py = custom_target('dll_copy', command: dllcopy, output: 'dll_copy_result')

    test('discrete', mgr_exe,
        args: [ server_exe.full_path(), client_exe.full_path() ],
        depends: [ client_exe, server_exe, move_dlls_py ])
endif
