# NOTE: several run of the same program to count average occurrence of different results.

import sys
from os.path import expanduser
sys.path.insert(0, "{}/projects/brun/src".format(expanduser("~")))

from brun import main, add, add_set

# add("ls /usr",                   # Command
#     { "name": "ls" })            # User-defined keys

# add("ls /nonsense",
#     { "name": "ls" })

# add("mpirun -np 3 ./a.out",
#     { "name": "mpi a"})

# # Make more test at once
# add_set("sleep {wait}",          # Command
#         { "name": "sleep" },     # Keys that are same for all
#         { "wait" : [ 0, 1, 3 ]}) # Keys to interpolate

def postprocess(context, result):
    print context
    if result["status"] != "ok":
        return
    with open("/tmp/brun") as f:
        result["result"] = f.readline().strip()

# for i in xrange(10):
add("mpirun -np 3 ./a.out > /tmp/brun",
    shell=True)
    # if (i % 10 == 0):
    #     add("sleep 1")

if __name__ == "__main__":
   main(post_fn=postprocess)
