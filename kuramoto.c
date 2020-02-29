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

	fp = fopen(argv[argc - 1], "r");
	for (i = 0; i < N; ++i)
		fscanf(fp, "%le\t%le\n", &theta[i], &omega[i]);
	for (i = N - 1; i >= 0; --i)
		omega[i] -= omega[0];
	fclose(fp);
	readadjl(adjlist, N, DIR);
	h = 1e-3;
	for (t = 0; t < T; ++t) {
		if (!(t % 100)) {
			print_osc(theta, N);
			printf("\n\n");
			fflush(stdout);
		}
		rk4(t * h, theta, N, kuramoto, h, theta);
	}
	return 0;
}
