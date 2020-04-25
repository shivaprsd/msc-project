#ifndef NETWORK_H
#define NETWORK_H

enum nwtype { DIR, UNDIR };
struct adjnode {
	struct adjnode *next;
	int id;
	int wt;
};
struct clique {
    struct clique *next;
    struct adjnode *nodes;
    int dim;
};
struct adjnode *addnode(struct adjnode **node, int id, int wt);
struct clique *addclq(struct clique **clq, struct adjnode *nodes, int dim);
int readadjl(FILE *fp, struct adjnode **adjlist, int lim, enum nwtype t);
void printadjl(struct adjnode **adjlist, int lim);
void freeadjl(struct adjnode **adjlist, int lim);
void printclqs(struct clique *clq, struct adjnode **adjlist);
int adjmtx(struct adjnode **adjlist, int i, int j);
void degdist(struct adjnode **adjlist, int lim);
void intersect(struct adjnode *a, struct adjnode *b, struct adjnode **out);
void bk(struct adjnode *r, struct adjnode *p, struct adjnode *x, struct adjnode
		**adjlist, int lim, struct clique **clqs);

void init_ring(int *ring, int n);
void plot_ring(int *ring, int n);
int adj(int *ring, int i, int j);
void rewire(int *ring, int n, double p);

#endif
