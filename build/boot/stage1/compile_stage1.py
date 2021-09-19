

import sys
inf = sys.argv[1]
outf = sys.argv[2]
reli = sys.argv[3]

import subprocess
output = subprocess.check_output("nasm {0} -f bin -i {2} -o {1}/stage1.bin".format(inf, outf, reli), shell=True)