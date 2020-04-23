#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "util.h"
#define HASH(n) (n - 1)

struct adjnode *addnode(struct adjnode **node, int id, int wt)
{
	struct adjnode *ap;

	ap = (struct adjnode *) malloc(sizeof(*ap));
	if (ap != NULL) {
		ap->id = id;
		ap->wt = wt;
		ap->next = *node;
		*node = ap;
	}
	return ap;
}

struct clique *addclq(struct clique **clq, struct adjnode *nodes, int dim)
{
	struct clique *cp;

	cp = (struct clique *) malloc(sizeof(*cp));
	if (cp != NULL) {
		cp->dim = dim;
		cp->nodes = nodes;
		cp->next = *clq;
		*clq = cp;
	}
	return cp;
}

int readadjl(struct adjnode **adjlist, int lim, enum nwtype t)
{
	int n1, n2, w, edgc;

	edgc = 0;
	while (scanf("%d %d %d", &n1, &n2, &w) != EOF) {
		if (n1 > lim || n2 > lim) {
			fprintf(stderr, "Node %d is out of limits!\n",
					(n1 > lim) ? n1 : n2);
			return -1;
		}
		++edgc;
		if ((t == UNDIR && addnode(&adjlist[HASH(n1)], n2, w) == NULL)
				|| addnode(&adjlist[HASH(n2)], n1, w) == NULL) {
			fprintf(stderr, "Memory allocation error!\n");
			return -1;
		}
	}
	return edgc;
}

void printadjl(struct adjnode **adjlist, int lim)
{
	struct adjnode *ap;
	int i;

	for (i = 0; i < lim; ++i) {
		printf("%d:", i + 1);
		for (ap = adjlist[i]; ap != NULL; ap = ap->next)
			printf(" %d (%d)%s", ap->id, ap->wt,
				(ap->next != NULL) ? "," : "");
		putchar('\n');
	}
}

void printclqs(struct clique *clq, struct adjnode **adjlist)
{
	int wt;
	struct adjnode *ap, *bp;

	for (; clq != NULL; clq = clq->next) {
		putchar('[');
		for (ap = clq->nodes; ap->next != NULL; ap = ap->next)
			printf("%d,", ap->id);
		printf("%d]: ", ap->id);
		for (ap = clq->nodes; ap != NULL; ap = ap->next) {
			for (bp = clq->nodes; bp != NULL; bp = bp->next) {
				if ((wt = adjmtx(adjlist, ap->id, bp->id)))
					printf("%d%c>%d ", ap->id,
						(wt == 2) ? '=' : '-', bp->id);
			}
		}
		putchar('\n');
	}
}

int adjmtx(struct adjnode **adjlist, int i, int j)
{
	struct adjnode *ap;

	for (ap = adjlist[HASH(j)]; ap != NULL; ap = ap->next)
		if (ap->id == i)
			return ap->wt;
	return 0;
}

void degdist(struct adjnode **adjlist, int lim)
{
	struct adjnode *ap;
	int i, d;

	for (i = 0; i < lim; ++i) {
		for (d = 0, ap = adjlist[i]; ap != NULL; ap = ap->next, ++d)
			;
		printf("%d %d\n", i + 1, d);
	}
}

void intersect(struct adjnode *a, struct adjnode *b, struct adjnode **ab)
{
	struct adjnode *t;

	for (; a != NULL; a = a->next) {
		for (t = b; t != NULL; t = t->next) {
			if (t->id == a->id) {
				if (addnode(ab, t->id, t->wt) != NULL)
					break;
				fprintf(stderr, "Memory allocation error!\n");
				return;
			}
		}
	}
}

int append(struct adjnode *s, struct adjnode **t) {
	int i;

	for (i = 0; s != NULL; s = s->next, ++i) {
		if (addnode(t, s->id, s->wt) == NULL) {
			fprintf(stderr, "Memory allocation error!\n");
			return -1;
		}
	}
	return i;
}

void bk(struct adjnode *r, struct adjnode *p, struct adjnode *x, struct adjnode
	**adjlist, int lim, struct clique **clqs)
{
	int n;
	struct adjnode *v, *pnew, *xnew;

	if (p == NULL && x == NULL) {
		v = NULL;
		if ((n = append(r, &v)) != -1)
			if (addclq(clqs, v, n) == NULL)
				fprintf(stderr, "Memory allocation error!\n");
		return;
	}
	while (p != NULL) {
		v = p;
		if ((n = v->id) > lim) {
			fprintf(stderr, "Node %d is out of limits!\n", n);
			return;
		}
		p = p->next;
		v->next = r;
		pnew = xnew = NULL;
		intersect(p, adjlist[HASH(n)], &pnew);
		intersect(x, adjlist[HASH(n)], &xnew);

		bk(v, pnew, xnew, adjlist, lim, clqs);
		v->next = x;
		x = v;
	}
}

void init_ring(int *ring, int n)
{
	int i;

	for (i = 0; i < n; ++i)
		ring[i] = (i + 1) % n;
}

void plot_ring(int *ring, int n)
{
	int i;

	for (i = 0; i < n; ++i)
		printf("%d\t%d\n", i + 1, ring[i] + 1);
}

int adj(int *ring, int i, int j)
{
	return (ring[i] == j || ring[j] == i);
}

void rewire(int *ring, int n, double p)
{
	int i, k;

	for (i = 0; i < n; ++i) {
		if (frand(0, 1) < p) {
			do
				k = (int) frand(0, n - EPS);
			while (k == i || ring[k] == i);
			ring[i] = k;
		}
	}
}

/*
int main()
{
	int i, j;
	int ring[N];

	init_ring();
	rewire(0.8);
	plot_ring();
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j)
			printf("%d ", adj(i, j));
		putchar('\n');
	}
	return 0;
}
*/
