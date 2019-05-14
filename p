#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np

AU = 149597900000

boxsize=2.5
filename='out.txt'
nb=3
niter=int(1e4)

x=[[] for _ in range(nb)]
with open(filename) as f:
    for i in range(niter):
        for j in range(nb):
            l = f.readline().split()
            x[j].append((float(l[0]),float(l[1])))

fig = plt.figure(figsize=(5,5))
ax = fig.add_subplot(111)
ax.set_aspect('equal')

plt.xlim(-boxsize/2*AU,boxsize/2*AU)
plt.ylim(-boxsize/2*AU,boxsize/2*AU)

for i in range(nb):
    xs = [x for (x,y) in x[i]]
    ys = [y for (x,y) in x[i]]
    plt.plot(xs,ys)

plt.show()
