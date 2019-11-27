#ifndef NETWORK_H
#define NETWORK_H

void init_ring(int *ring, int n);
void plot_ring(int *ring, int n);
int adj(int *ring, int i, int j);
void rewire(int *ring, int n, double p);

#endif
