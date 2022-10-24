IDIR =../include
CC=gcc
CFLAGS=-O3

ODIR=bin

DEPS = bitmap.h

_OBJ = sieve sieve_pthreads
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: sieve sieve_pthreads

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $(ODIR)/$@ $<

sieve: sieve.o bitmap.o
	$(CC) $(CFLAGS) sieve.c bitmap.c -o $(ODIR)/$@

sieve_pthreads: sieve_pthreads.o bitmap.o
	$(CC) $(CFLAGS) sieve_pthreads.c bitmap.c -o $(ODIR)/$@

.PHONY: clean

clean:
	rm -f $(ODIR)/* *~ core $(INCDIR)/*~
