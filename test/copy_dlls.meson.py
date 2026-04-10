import shutil
import os
import sys
import platform
import subprocess

if __name__ == "__main__":
    fname = sys.argv[1]
    fdest = os.path.join(sys.argv[2], os.path.basename(sys.argv[1]))

    shutil.copyfile(fname, fdest)
    os.chmod(fdest, 0o755)

    # if for some reason
    if (platform.system() == "Linux"):
        subprocess.run(["patchelf", "--set-rpath", "$ORIGIN", fdest])
