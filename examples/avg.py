from results import results

sum = 0
count = 0

for r in results:
    sum += r["time"]
    count += 1


print "Average time is: {}".format(sum / count)
