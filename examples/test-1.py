# NOTE: several run of the same program to count average occurrence of different results.

import sys
from os.path import expanduser
sys.path.insert(0, "{}/projects/brun/src".format(expanduser("~")))

from brun import main, add, add_set

def postprocess(context, result):
    print context
    if result["status"] != "ok":
        return
    with open("/tmp/brun") as f:
        result["result"] = f.readline().strip()

add("mpirun -np 10 ./a.out > /tmp/brun",
    shell=True)

if __name__ == "__main__":
   main(post_fn=postprocess)
