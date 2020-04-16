#import numpy as np
import sys
f = open(sys.argv[-1], "r")
g = [[int(x) for x in l.split(" ")] for l in f.readlines()]
f.close()
n = max(max(l[0] for l in g), max(l[1] for l in g))
#amtx = np.zeros([n + 1, n + 1], int)
alist = {}
dlist = {}
for i in range(1, n + 1):
    alist[i] = []
    dlist[i] = []

for k in range(len(g)):
    i, j, w = g[k][0], g[k][1], g[k][2]
    alist[i].append(j)
    alist[j].append(i)
    dlist[j].append([i, w])
    #amtx[i][j] = amtx[j][i] = 1

def print_clq(clq):
    print "%s:\t" % list(clq),
    for i in clq:
        for j in clq:
            for x in dlist[j]:
                if i == x[0]:
                    print "%d-%s%d " % (i, '>' * x[1], j),
    print

def bk(r, p, x):
    if p or x:
        for v in p.copy():
            nv = set(alist[v])
            v = set([v])
            bk(r | v, p & nv, x & nv)
            p -= v
            x |= v
    else:
        print_clq(r)

bk(set(), set(range(1, n + 1)), set())
