#!/usr/bin/python3

import argparse
from matplotlib import pyplot as plt
from matplotlib import animation
import numpy as np

parser = argparse.ArgumentParser(description='Plots the output of cnbm.')
parser.add_argument('-a', help='Animates the plot.',action='store_true')
args = parser.parse_args()

AU = 149597900000

boxsize=5
filename='out.txt'
nb=3
niter=int(5e4)
nskip=10

nf = int(niter/(nskip+1))

x=[[] for _ in range(nb)]
with open(filename) as f:
    for i in range(int(niter/(nskip+1))):
        for j in range(nb):
            l = f.readline().split()
            x[j].append((float(l[0]),float(l[1])))
        for j in range(nb):
            for k in range(nskip):
                f.readline()

fig = plt.figure(figsize=(5,5))
ax = fig.add_subplot(111)
ax.set_aspect('equal')

plt.xlim(-boxsize/2*AU,boxsize/2*AU)
plt.ylim(-boxsize/2*AU,boxsize/2*AU)

if args.a:
    lines=[]
    for i in range(nb):
        line, = ax.plot([],[],linestyle='none',marker='o')
        lines.append(line)

    def init():
        for el in lines:
            el.set_data([],[])
        return lines

    def animate(i):
        dat = [x[j][i] for j in range(nb)]
        xs = [x for (x,y) in dat]
        ys = [y for (x,y) in dat]
        for j,el in enumerate(lines):
            el.set_data(x[j][i])
        return lines

    anim = animation.FuncAnimation(fig,animate,init_func=init,frames=nf,interval=5,blit=True)

else:
    for i in range(nb):
        xs = [x for (x,y) in x[i]]
        ys = [y for (x,y) in x[i]]
        plt.plot(xs,ys)

plt.show()
