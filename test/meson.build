dllcopy_libtestrig = [find_program('python'),
    meson.project_source_root() / 'test' / 'copy_dlls.meson.py',
    lib.full_path(),
    meson.project_source_root() / 'build' / 'test']

dllcopy_ftd2xx = [find_program('python'),
    meson.project_source_root() / 'test' / 'copy_dlls.meson.py',
    where_ftd2xx,
    meson.project_source_root() / 'build' / 'test']

dllcopy_epos2 = [find_program('python'),
    meson.project_source_root() / 'test' / 'copy_dlls.meson.py',
    where_epos2,
    meson.project_source_root() / 'build' / 'test']

_move_dlls_tests1 = custom_target('_dll_copy1', command: dllcopy_libtestrig, output: 'dll_copy_result1')
_move_dlls_tests2 = custom_target('_dll_copy2', command: dllcopy_epos2, output: 'dll_copy_result2')
_move_dlls_tests3 = custom_target('_dll_copy3', command: dllcopy_ftd2xx, output: 'dll_copy_result3')

_test_deps = [ _move_dlls_tests1, _move_dlls_tests2, _move_dlls_tests3 ]

sock_name_exe = executable('test_sock_name', 'test_sock_name.c', dependencies: dep)
test('sockname', sock_name_exe, depends: [ _test_deps ])

if (operating_system.to_lower() == 'linux')
    fork_exe = executable('test_exe', 'test_fork_ipc.c',
        dependencies: dep,
        build_rpath: std_rpath)
    preset_path_exe = executable('preset_path_exe', 'test_preset_sockpath.c',
        dependencies: dep,
        build_rpath: std_rpath)

    test('forking', fork_exe, depends: [ _test_deps ])
    test('preset_path', preset_path_exe, depends: [ _test_deps ])

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


    test('discrete', mgr_exe,
        args: [ server_exe.full_path(), client_exe.full_path() ],
        depends: [ client_exe, server_exe, _test_deps ])
endif
