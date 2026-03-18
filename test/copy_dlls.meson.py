import shutil
import os
import sys

if __name__ == "__main__":
    shutil.copyfile(
        sys.argv[1],
        os.path.join(sys.argv[2], os.path.basename(sys.argv[1])))