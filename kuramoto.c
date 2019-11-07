#include <stdio.h>
#include <math.h>
#include "util.h"
#define N 100
#define K 2.3
#define SQR(x) (pow(x, 2))

double omega[N];

void kuramoto(double t, double theta[], double thdot[])
{
	int i, j;

	for (i = 0; i < N; ++i) {
		thdot[i] = omega[i];
		for (j = 0; j < N; ++j)
			thdot[i] += K / N * sin(theta[j] - theta[i]);
	}
}

double ord_param(double theta[])
{
	int i;
	double sr, si;

	sr = si = 0;
	for (i = 0; i < N; ++i) {
		sr += cos(theta[i]);
		si += sin(theta[i]);
	}
	sr /= N;
	si /= N;
	return sqrt(sr * sr + si * si);
}

void print_osc(double theta[])
{
	int i, c;

	for (i = 0; i < N; ++i) {
		c = (sin(theta[i]) > 0.98) ? 7 : 8;
		printf("%e\t%e\t%d\n", cos(theta[i]), sin(theta[i]), c);
	}
	printf("\n\n");
}

int main()
{
	int t, i;
	double h, theta[N];
	const double sig_sqr = N * N / (8 * log(2));

	for (i = 0; i < N; ++i) {
		theta[i] = frand(0, 2 * M_PI);
		omega[i] = 2 * M_PI * exp(-SQR(i - N / 2) / (2 * sig_sqr));
	}
	printf("#K = %.2lf, g(w): Gaussian\n", K);
	h = 1e-3;
	for (t = 0; t < 2e4; ++t) {
		rk4(t * h, theta, N, kuramoto, h, theta);
		if (t % (int) (0.02 / h + 0.5) == 0)
			print_osc(theta);
	}
	return 0;
}
