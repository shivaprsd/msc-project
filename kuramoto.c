#include <stdio.h>
#include <math.h>
#include "network.h"
#include "util.h"
#define N 1000

double K, omega[N];
struct adjnode *adjlist[N] = { NULL };

void kuramoto(double t, double theta[N], double thdot[N])
{
	int i;
	struct adjnode *ap;

	for (i = 0; i < N; ++i) {
		thdot[i] = omega[i];
		for (ap = adjlist[i]; ap != NULL; ap = ap->next)
			thdot[i] += K * sin(theta[ap->id - 1] - theta[i]);
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

void print_osc(double *theta, unsigned int n)
{
	int i;

	for (i = 0; i < n; ++i)
		printf("%e\t%e\n", cos(theta[i]), sin(theta[i]));
}

int main()
{
	int t, i;
	const int T = 5e3;
	double r, h, theta[N];

	for (i = 0; i < N; ++i) {
		theta[i] = frand(0, 2 * M_PI);
		omega[i] = gauss(2 * M_PI, M_PI / 6);
	}
	readadjl(adjlist, N, UNDIR);
	h = 1e-3;
	puts("# Coupling strength K\tOrder parameter |r|");
	for (K = 1; K >= 0; K -= 0.05) {
		for (t = 0; t < T; ++t)
			rk4(t * h, theta, N, kuramoto, h, theta);
		r = 0.0;
		for (t = 0; t < T; ++t) {
			r += ord_param(theta, N);
			rk4(t * h, theta, N, kuramoto, h, theta);
		}
		fprintf(stderr, "%.2lf\t%.7e\n", K, r / T);
		printf("%.2lf\t%.7e\n", K, r / T);
		fflush(stdout);
	}
	return 0;
}
