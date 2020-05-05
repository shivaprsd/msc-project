#include <stdio.h>
#include <math.h>
#include "network.h"
#include "util.h"
#define N 1000

double omega[N];
struct adjnode *adjlist[N] = { NULL };

void kuramoto(double t, double theta[N], double thdot[N])
{
	int i;
	struct adjnode *ap;

	for (i = 0; i < N; ++i) {
		thdot[i] = omega[i];
		for (ap = adjlist[i]; ap != NULL; ap = ap->next)
			thdot[i] += ap->wt * sin(theta[ap->id - 1] - theta[i]);
	}
}

double ord_param(double *theta, unsigned int n)
{
	int i;
	double sr, si;

	sr = si = 0;
	for (i = 0; i < n; ++i) {
		sr += cos(theta[i]);
		si += sin(theta[i]);
	}
	return sqrt(sr * sr + si * si) / n;
}

double clq_ord(struct clique *clq, double *theta)
{
	double sr, si, th;
	struct adjnode *ap;

	sr = si = 0;
	for (ap = clq->nodes; ap != NULL; ap = ap->next) {
		th = theta[ap->id - 1];
		sr += cos(th);
		si += sin(th);
	}
	return sqrt(sr * sr + si * si) / clq->dim;
}

enum bool has_node(struct adjnode *n, int id)
{
	for (; n != NULL; n = n->next)
		if (n->id == id)
			return true;
	return false;
}

double thwrap(double th)
{
	return ((th = fmod(th, 2 * M_PI)) > 0) ? th : th + 2 * M_PI;
}

void print_osc(double *theta, unsigned int n)
{
	int i;

	for (i = 0; i < n; ++i)
		printf("%le\n", thwrap(theta[i]));
}

int main(int argc, char *argv[])
{
	int t, i;
	const int T = 1e5;
	double r, h, theta[N];
	FILE *fp;
	struct adjnode *p = NULL;
	struct clique *c, *clq = NULL;

	fp = fopen(argv[argc - 2], "r");
	readadjl(fp, adjlist, N, UNDIR);
	for (i = 1; i <= N; ++i)
		addnode(&p, i, 1);
	bk(NULL, p, NULL, adjlist, N, &clq);
	freeadjl(adjlist, N);
	rewind(fp);
	readadjl(fp, adjlist, N, DIR);
	fclose(fp);

	fp = fopen(argv[argc - 1], "r");
	for (i = 0; i < N; ++i)
		fscanf(fp, "%le\t%le\n", &theta[i], &omega[i]);
	//for (i = N - 1; i >= 0; --i)
	//	omega[i] -= omega[0];
	fclose(fp);
	h = 1e-3;
	for (t = 0; t < T; ++t) {
		if (!(t % 100)) {
			for (c = clq; c != NULL; c = c->next)
				printf("%le\t%d\n", clq_ord(c, theta),
					(has_node(c->nodes, 1) ? 7 : 6));
			printf("\n\n");
			fflush(stdout);
		}
		rk4(t * h, theta, N, kuramoto, h, theta);
	}
	return 0;
}
