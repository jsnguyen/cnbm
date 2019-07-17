#!/usr/bin/python3

import argparse
from matplotlib import pyplot as plt
from matplotlib import animation

parser = argparse.ArgumentParser(description='Plots the output of cnbm.')
parser.add_argument('--filename=', dest='filename', nargs='?', const=1, default='out.txt', help='plotting filename', type=str)
parser.add_argument('-a', help='Animates the plot.',action='store_true')
parser.add_argument('-s', help='Saves animation of the plot into several image files for turning into a gif',action='store_true')
args = parser.parse_args()

AU = 149597900000

boxsize=5
filename=args.filename
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

fig = plt.figure(figsize=(3,3))
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
    plt.show()

elif args.s:
    # ugly, but saves frames for turning into gif
    nSave = 500
    xxs=[]
    yys=[]
    for i in range(nb):
        xs = [x for (x,y) in x[i]]
        ys = [y for (x,y) in x[i]]
        xxs.append(xs)
        yys.append(ys)

    counter=0;
    for j in range(0,len(xxs[0]),int(len(xxs[0])/nSave)):
        plt.xlim(-boxsize/2*AU,boxsize/2*AU)
        plt.ylim(-boxsize/2*AU,boxsize/2*AU)
        plt.axis('off')
        for i in range(nb):
            plt.plot(xxs[i][j],yys[i][j],linestyle='none',marker='o')
        pad=''
        if counter<100:
            pad='0'
        if counter<10:
            pad='00'
        saveFilename = 'plot_'+pad+str(counter)+'.png'
        print('Saving figure '+saveFilename)
        plt.savefig(saveFilename,bbox_inches='tight')
        counter+=1
        plt.clf()
    print('Done')


else:
    for i in range(nb):
        xs = [x for (x,y) in x[i]]
        ys = [y for (x,y) in x[i]]
        plt.plot(xs,ys,linestyle='none',marker='o')

    plt.show()
