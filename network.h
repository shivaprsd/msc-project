#ifndef NETWORK_H
#define NETWORK_H

enum nwtype { DIR, UNDIR };
struct adjnode {
	struct adjnode *next;
	int id;
	int wt;
};
struct adjnode *install(struct adjnode **node, int id, int wt);
int readadjl(struct adjnode **adjlist, int lim, enum nwtype t);
void printadjl(struct adjnode **adjlist, int lim);
void degdist(struct adjnode **adjlist, int lim);

void init_ring(int *ring, int n);
void plot_ring(int *ring, int n);
int adj(int *ring, int i, int j);
void rewire(int *ring, int n, double p);

#endif
