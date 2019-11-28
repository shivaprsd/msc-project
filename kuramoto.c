#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "util.h"
#define N 100
#define SQR(x) (pow(x, 2))

double K, omega[N];

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
#pragma omp parallel for private(t, r) firstprivate(theta)
	for (i = 0; i <= 20 ; ++i) {
		K = i * 0.05;
		for (t = 0; t < 1e3; ++t)
			rk4(t * h, theta, N, kuramoto, h, theta);
		for (r = 0.0; t < 1e5; ++t) {
			rk4(t * h, theta, N, kuramoto, h, theta);
			r += ord_param(theta, N);
		}
#pragma omp critical
		printf("%e\t%e\n", K, r / 1e5);
	}
	return 0;
}
