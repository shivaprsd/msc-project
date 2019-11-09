#include <stdio.h>
#include <math.h>
#include "util.h"
#define N 100
#define K 2.3
#define SQR(x) (pow(x, 2))

double omega[N];

void kuramoto(double t, double theta[N], double thdot[N])
{
	int i, j;

	for (i = 0; i < N; ++i) {
		thdot[i] = omega[i];
		for (j = 0; j < N; ++j)
			thdot[i] += K / N * sin(theta[j] - theta[i]);
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
	int i, c;

	for (i = 0; i < n; ++i) {
		c = (sin(theta[i]) > 0.98) ? 7 : 8;
		printf("%e\t%e\t%d\n", cos(theta[i]), sin(theta[i]), c);
	}
	printf("\n\n");
}

int main()
{
	int t, i;
	double r, h, theta[N];

	for (i = 0; i < N; ++i) {
		theta[i] = frand(0, 2 * M_PI);
		omega[i] = gauss(2 * M_PI, M_PI / 12);
	}
	h = 1e-3;
	for (t = 0; t < 1e3; ++t)
		rk4(t * h, theta, N, kuramoto, h, theta);
	for (r = 0.0; t < 2e4; ++t) {
		rk4(t * h, theta, N, kuramoto, h, theta);
		if (t % (int) (0.02 / h + 0.5) == 0)
			print_osc(theta, N);
		r += ord_param(theta, N);
	}
	fprintf(stderr, "%e\t%e\n", K, r / 1e5);
	return 0;
}
