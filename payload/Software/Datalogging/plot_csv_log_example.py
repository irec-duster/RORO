import csv
import matplotlib.pyplot as plt
import sys

f = open(sys.argv[1], 'r')
csv_reader = csv.reader(f)
# data = [row for row in csv_reader]
data = list(csv_reader)

time_index = data[0].index('TIME_StartTime')

legend = data[0]
legend.pop(time_index)

data_vect = list(zip(*data[1:]))
time = data_vect[time_index]
data_vect.pop(time_index)

fig = plt.figure()
ax = fig.add_subplot(111, axisbg = 'w')
for x in data_vect:
    ax.plot(time, x)
ax.legend(legend)
plt.show()