import matplotlib.pyplot as plt
import numpy as np

file = open('timing', 'r')
Lines = file.readlines()

timings = []

for line in Lines:
	line = line.split(',')
	#print(int(line[len(line)-1]))
	timings.append(int(line[len(line)-1]))

min_ = min(timings)
max_ = max(timings)
number_buckets = 20
range_ = (max_ - min_)/number_buckets

ranges = [int(min_+i*range_) for i in range(number_buckets+1)]
timing_buckets = [0 for i in range(number_buckets+1)]

for time in timings:
	index = (time - min_)//range_
	timing_buckets[int(index)] += 1

plt.style.use('ggplot')
x_pos = [i for i in range(number_buckets+1)]
plt.bar(x_pos, timing_buckets, color='green')
plt.xticks(x_pos, ranges, rotation=65)
plt.show()
"""
import matplotlib.pyplot as plt

plt.style.use('ggplot')

x = ['Nuclear', 'Hydro', 'Gas', 'Oil', 'Coal', 'Biofuel']
energy = [5, 6, 15, 22, 24, 8]

x_pos = [i for i, _ in enumerate(x)]

plt.bar(x_pos, energy, color='green')
plt.xlabel("Energy Source")
plt.ylabel("Energy Output (GJ)")
plt.title("Energy output from various fuel sources")

plt.xticks(x_pos, x)

plt.show()
"""