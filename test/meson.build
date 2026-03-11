if (operating_system.to_lower() == 'linux')
    fork_exe = executable('test_exe', 'test_fork_ipc.c', dependencies: dep)
    preset_path_exe = executable('preset_path_exe', 'test_preset_sockpath.c', dependencies: dep)

    test('forking', fork_exe)
    test('preset_path', preset_path_exe)
elif (operating_system.to_lower() == 'windows')
    message('No tests exist for Windows')
endif