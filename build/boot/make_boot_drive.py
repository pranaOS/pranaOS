

import sys
inf = sys.argv[1]
outf = sys.argv[2]
reli = sys.argv[3]

import subprocess
output = subprocess.check_output("cat {0} {1} > {2}".format(inf, outf, reli), shell=True)