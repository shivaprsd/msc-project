#include <stdio.h>
#include <math.h>
#include "network.h"
#include "util.h"
#define N 50
#define K 5.0
#define SQR(x) (pow(x, 2))

double omega[N];
int ring[N];

void kuramoto(double t, double theta[N], double thdot[N])
{
	int i, j;

	for (i = 0; i < N; ++i) {
		thdot[i] = omega[i];
		for (j = 0; j < N; ++j) {
			if (adj(ring, i, j))
				thdot[i] += K * sin(theta[j] - theta[i]);
		}
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
	const int T = 5e4;
	const double p = 0.3;
	double r, h, theta[N];

	for (i = 0; i < N; ++i) {
		theta[i] = frand(0, 2 * M_PI);
		omega[i] = gauss(2 * M_PI, M_PI / 12);
	}
	init_ring(ring, N);
	rewire(ring, N, p);
	h = 1e-3;
	printf("#K = %.2lf, Regular ring\n", K);
	plot_ring(ring, N);
	printf("\n\n");
	for (t = r = 0; t < T; ++t) {
		if (t % (int) (0.02 / h + 0.5) == 0) {
			print_osc(theta, N);
			printf("\n\n");
		}
		if (t >= T / 2)
			r += ord_param(theta, N);
		rk4(t * h, theta, N, kuramoto, h, theta);
	}
	fprintf(stderr, "%.2lf\t%.2lf\n", K, 2 * r / T);
	return 0;
}
