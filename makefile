sim: kuramoto.c network.o util.o
	gcc-9 -static-libgcc -fopenmp -o sim kuramoto.c network.o util.o

evol: kuramoto.c network.o util.o
	cc -o evol kuramoto.c network.o util.o

network.o: network.c network.h util.o
	cc -c network.c

util.o: util.c util.h
	cc -c util.c

clean:
	rm sim evol network.o util.o
