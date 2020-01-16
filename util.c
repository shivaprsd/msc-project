/* Utilities for genral purpose programming */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "util.h"

/* Dynamically allocate an n element array of doubles */
double *vector(int n)
{
	double *v;

	v = (double *) malloc((size_t) n * sizeof(double));
	return v;
}

/* Dynamically allocate an m x n matrix of doubles */
double **matrix(int m, int n)
{
	int i;
	double **a;

	a = (double **) malloc((size_t) m * sizeof(double *));
	if (n) {
		for (i = 0; i <	m; ++i)
			a[i] = vector(n);
	}
	return a;
}

/* Set all the elements of vector v to k */
void set(double *v, int n, double k)
{
	for (; n > 0; --n)
		*v++ = k;
}

/* Compute inner-product of two vectors: u.v */
double dot(double *u, double *v, int n)
{
	double s;

	for (s = 0.0; n > 0; --n)
		s += *u++ * *v++;
	return s;
}

/* Multiply vector by a scalar k */
void scale(double *v, int n, double k)
{
	for (; n > 0; --n)
		*v++ *= k;
}

/* Return the projection of v along u as a dynamically allocated vector *
 * proj(v, u) = v.u * u / u.u */
double *proj(double *v, double *u, int n)
{
	double a, *t;

	t = vector(n);
	copy_vec(u, t, n);
	a = dot(u, u, n);
	if (a == 0.0)
		scale(t, n, 0.0);	/* projection along zero vector = 0 */
	else
		scale(t, n, dot(u, v, n) / a);
	return t;
}

/* Subtract vector u from v: v - u */
void sub(double *v, double *u, int n)
{
	for (; n > 0; --n)
		*v++ -= *u++;
}

/* Normalize given vector v and return its original norm */
double norm(double *v, int n)
{
	double a;

	a = sqrt(dot(v, v, n));
	if (a != 0.0) {		/* don't normalize zero vector */
		for (; n > 0; --n)
			*v++ /= a;
	}
	return a;
}

/* Print the elements of an array of size n in given format */
void print_vec(double *v, int n, const char *fmt)
{
	for (; n > 0; --n)
		printf(fmt, *v++);
	putchar('\n');
}

/* Copy array s to t, both having size n */
void copy_vec(double *s, double *t, int n)
{
	for (; n > 0; --n)
		*t++ = *s++;
}

/* Calculate the n dimensional distance between two vectors */
double dist(double *v1, double *v2, int n)
{
	double d;

	for (d = 0.0; n > 0; --n)
		d += pow((*v1++ - *v2++), 2);
	return sqrt(d);
}

/* Insert integer <val> in array <q> of size <n> at the index <pos>, pushing
 * the remaining elements back & discarding the last element, as in a 'queue' */
void ins(int val, int *q, int n, int pos)
{
	for (--n; n > pos; --n)
		q[n] = q[n - 1];
	q[pos] = val;
}

/* Same as ins(), for double arrays */
void fins(double val, double *q, int n, int pos)
{
	for (--n; n > pos; --n)
		q[n] = q[n - 1];
	q[pos] = val;
}

/* Compute the root-mean-squared deviation (uncorrected for bias) of a set of
 * data <x> of size <n> by Two-pass algorithm */
double rmsd(double *x, int n)
{
	int i;
	double s, xm;

	for (s = i = 0; i < n; s += x[i++])	/* first pass to find mean */
		;
	xm = s / n;
	for (s = i = 0; i < n; ++i)		/* second pass */
		s += pow(x[i] - xm, 2);
	return sqrt(s / n);
}

/* Return a random real number between lb and ub (both inclusive).
 * Employs the Bays-Durham shuffling algorithm as given in Num. Recep. */
double frand(double lb, double ub)
{
	int i, r;
	static int p = -1;		/* next rand no. chooser */
	static int deck[DECK_SIZE];	/* shuffling table */

	if (p < 0) {
		srand(time(NULL));
		/* Load the table after 8 warmups */
		for (i = -8; i < DECK_SIZE; ++i) {
			r = rand();
			if (i >= 0) deck[i] = r;
		}
		p = rand();
	}
	i = (int) (p / (RAND_MAX + 1.0) * DECK_SIZE);	/* i = 0...31 */
	p = deck[i];		/* next chooser */
	deck[i] = rand();	/* refill vacancy */
	return (ub - lb) * p / RAND_MAX + lb;	/* scale & return */
}

/* Return a random number from the Gaussian distribution centred at <mu> having
 * a std. deviation <sig>. Employs the Marsaliga / Box-Muller polar method. */
double gauss(double mu, double sig)
{
	double u, s;
	static double v;
	static enum bool gen = false;
	gen = !gen;			/* generate afresh every other call */

	if (!gen)
		return v * sig + mu;	/* return spare */
	/* Get a random point (u, v) within the unit circle centred at origin */
	do {
		u = frand(-1, 1);
		v = frand(-1, 1);
		s = u * u + v * v;
	} while (s >= 1.0 || s == 0.0);
	s = sqrt(-2 * log(s) / s);	/* polar scaling */
	v *= s;				/* store spare & return the other */
	return u * s * sig + mu;
}

/* Compare two real numbers; tolerance can be defined as EPS */
enum bool eq(double x, double y)
{
	if (fabs(x - y) < EPS)
		return true;
	else
		return false;
}

/* n dimensional RK4 algorithm for calculating a single step.
 * This implementation is loosely based on the one given in Numerical Recipes.
 * As we cannot multiply arrays with numbers in C (without using a loop), an
 * approach different from the traditional way (k1, k2, ...) has been followed.
 */
void rk4(double x, double y[], int n, deriv_func f, double h, double yout[])
{
	int i;
	double hh, xh, *dy, *dys, *yt;

	yt = vector(n);
	dy = vector(n);		/* derivatives of y1, y2, ... */
	dys = vector(n);	/* sum of derivatives */
	hh = 0.5 * h;
	xh = x + hh;

	f(x, y, dy);				/* first step */
	for (i = 0; i < n; ++i) {
		yt[i] = y[i] + hh * dy[i];	/* y + k1 / 2 */
		dys[i] = dy[i];
	}
	f(xh, yt, dy);				/* second step */
	for (i = 0; i < n; ++i) {
		yt[i] = y[i] + hh * dy[i];	/* y + k2 / 2 */
		dys[i] += 2 * dy[i];
	}
	f(xh, yt, dy);				/* third step */
	for (i = 0; i < n; ++i) {
		yt[i] = y[i] + h * dy[i];	/* y + k3 */
		dys[i] += 2 * dy[i];
	}
	f(x + h, yt, dy);			/* fourth step */
	for (i = 0; i < n; ++i)
		yout[i] = y[i] + h / 6.0 * (dys[i] + dy[i]);	/* final y */

	free(yt);
	free(dy);
	free(dys);
}

/* Orthonormalize a set of <m> n-dimensional vectors by Gram–Schmidt procedure *
 * and store their original norms in <a> */
void ortho(double **u, int m, int n, double *a)
{
	int i, k;
	double *p;

	for (k = 0; k < m; ++k) {
		/* Subtract components along all u[i < k] from u[k] */
		for (i = 0; i < k; ++i) {
			p = proj(u[k], u[i], n);
			sub(u[k], p, n);
			free(p);
		}
		/* Normalize u[k] & store its original norm */
		a[k] = norm(u[k], n);
	}
}

/* QR decompose an m x n matrix <A> (with m >= n) into m x n <Q> and n x n <R>.
 * All the three matrices are expected to be in column-major order. */
void qrd(double **a, double **q, double **r, int m, int n)
{
	int i, j;
	double *t;

	for (i = 0; i < n; ++i) {
		copy_vec(a[i], q[i], m);
		set(r[i], n, 0.0);
	}
	t = vector(n);
	ortho(q, n, m, t);		/* orthonormalize Q */
	/* R = Q'A = dot(Q, A); but we need to compute only for j >= i */
	for (i = 0; i < n; ++i) {
		r[i][i] = t[i];
		for (j = i + 1; j < n; ++j)
			r[j][i] = dot(q[i], a[j], m);
	}
	free(t);
}

/* Solve matrix equation Rx = b by back substitution in-place (i.e. converting
 * <b> to the solution vector <x>), assuming solution exists. R should be n x n
 * in column-major order; b should be an n-element vector. */
void bsub(double **r, int n, double *b)
{
	int i, j;

	for (i = n - 1; i >= 0; --i) {		/* start from last row */
		for (j = i + 1; j < n; ++j)
			b[i] -= b[j] * r[j][i];
		b[i] /= r[i][i];
	}
}
