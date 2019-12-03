#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "util.h"
#define HASH(n) (n - 1)

struct adjnode *install(struct adjnode **node, int id, int wt)
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

int readadjl(struct adjnode **adjlist, int lim)
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
		if (install(&adjlist[HASH(n1)], n2, w) == NULL
				|| install(&adjlist[HASH(n2)], n1, w) == NULL) {
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
