
from results import results

count = {}

for r in results:
    if r["result"] not in count:
        count[r["result"]] = 1
    else:
        count[r["result"]] += 1

for k, v in count.iteritems():
    print "{}: {}".format(k, v)
