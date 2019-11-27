#include <stdio.h>
#include "network.h"
#include "util.h"

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
