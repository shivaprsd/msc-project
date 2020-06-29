#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "network.h"
#include "util.h"
#define N 1000

double omega[N], om0;
struct adjnode *adjlist[N] = { NULL };
#pragma omp threadprivate(om0)

void kuramoto(double t, double theta[N], double thdot[N])
{
	int i;
	struct adjnode *ap;

	for (i = 0; i < N; ++i) {
		thdot[i] = omega[i];
		for (ap = adjlist[i]; ap != NULL; ap = ap->next)
			thdot[i] += ap->wt * sin(theta[ap->id - 1] - theta[i]);
	}
	thdot[0] = om0;
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

int main(int argc, char *argv[])
{
	int t, i, j;
	const int T = 5e4;
	char s[13];
	double r, h, theta[N], theta0[N];
	const double sigma = M_PI / 2;
	FILE *fp;

	fp = fopen(argv[argc - 1], "r");
	for (i = 0; i < N; ++i) {
		fscanf(fp, "%le\t%le\n", &theta0[i], &omega[i]);
		omega[i] -= 2 * M_PI;
	}
	fclose(fp);
	readadjl(adjlist, N, DIR);
	h = 1e-3;

#pragma omp parallel for private(i, t, r, s, fp, theta)
	for (j = 0; j <= 400; ++j) {
		om0 = (j - 200) * M_PI / 200;
		copy_vec(theta0, theta, N);
		for (t = 0; t < T; ++t)
			rk4(t * h, theta, N, kuramoto, h, theta);
		for (t = 0; t < T; ++t) {
			if (!(t % 50)) {
				r = ord_param(theta, N);
#pragma omp critical
				printf("%d\t%lf\t%lf\n", j, r, t * h);
			}
			rk4(t * h, theta, N, kuramoto, h, theta);
		}
	}
	return 0;
}
