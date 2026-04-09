#!/usr/bin/env python

import os
import platform
import shutil
import subprocess
import time

from os.path import join, abspath
dirs = {
    "repo_root": abspath(join(__file__, "..", "..")),
    "lib_dir": abspath(join(__file__, ".."))
}

listing_link = "https://www.maxongroup.us/maxon/view/product/control/Positionierung/375711"
windows_link = "https://www.maxongroup.us/medias/sys_master/root/9443687104542/EPOS-Windows-DLL-En.zip"
linux_link = "https://www.maxongroup.us/medias/sys_master/root/9443687202846/EPOS-Linux-Library-En.zip"

def get_mfg(arch: str) -> str:
    return "intel" if "x86" in arch else "arm"

def get_curl(operating: str) -> str:
    return "curl" if "Linux" else "curl.exe"

def attempt_dl(link: str, outf: str|os.PathLike, operating: str) -> int:
    try:
        lib_get = subprocess.run([f"{get_curl(operating)} \"\"{link}\"\" --output \"\"{outf}\"\""], shell=True)

    except Exception as excepter:
        print(excepter)
        return 1

    return lib_get.returncode

if __name__ == "__main__":
    i_am = platform.system()

    os.makedirs(dirs["lib_dir"], exist_ok=True)

    archive_name = join(dirs["lib_dir"], "libepos.zip")
    attempt_code: int
    arch: str = "x86_64"
    libname: str = ""
    other_libname: str = ""
    dirname: str = ""
    extension: str = ""

    show_eula = input("The binary to install is property of maxon group. " \
                      "By using this tool, you agree to the terms laid out by the EULA " \
                      "that is included inside of the archive.")

    if i_am == "Windows":
        attempt_code = attempt_dl(windows_link, archive_name, i_am)
        libname = "EposCmd64.dll"
        other_libname = "EposCmd64.lib"
        dirname = "Microsoft Visual C++"
        extension = ".dll"

    elif i_am == "Linux":
        attempt_code = attempt_dl(linux_link, archive_name, i_am)
        libname = "libEposCmd.so.6.8.1.0"
        other_libname = "libftd2xx.so"
        dirname = arch
        extension = ".so"

    if (attempt_code != 0):
        print(f"Please visit {listing_link} for a manual download.")
        exit(1)
    
    print("Done downloading!")
    print("Unzipping contents...") 

    wherego = join(dirs["lib_dir"], "epos_temp")
    shutil.unpack_archive(archive_name, wherego)

    print("Processing contents...")
    bins = [ ]

    for (root, dirnames, files) in os.walk(wherego):
        for f in files:
            if "EULA.txt" in f:
                os.rename(join(root, f), join(dirs["lib_dir"], f))
                continue

            if (not dirname in root):
                continue
            if (libname in f or other_libname in f):
                stripped = f.split(extension)[0] + extension
                actual_bin = stripped if i_am == "Linux" else f

                wherestrip = join(dirs["lib_dir"], stripped)
                os.rename(join(root, f), wherestrip)
                bins.append(wherestrip)

                if (i_am == "Linux"):
                    os.chmod(wherestrip, 0o755)

    shutil.rmtree(wherego)
    os.remove(archive_name)
    if (i_am == "Linux"):
        for binary in bins:
            subprocess.run(["patchelf", "--set-rpath", "$ORIGIN", binary])

    where_eula = join(dirs["lib_dir"], "EULA.txt")
    print(f"The following can be found at {where_eula}")

    with open(where_eula, "r") as f:
        for line in f.readlines():
            print(line)

    print(f"Done! Make sure to read the maxon EULA at {where_eula}")
    exit(0)
