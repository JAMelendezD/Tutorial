SRC = ../src
IDIR = ../include
CC = gcc
CFLAGS = -g -O2 -I$(IDIR)

ODIR = ../obj
LDIR = ../lib

LIBS =  -lm

_DEPS = solutions.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ_main = solutions.o euler.o
OBJ_main = $(patsubst %,$(ODIR)/%,$(_OBJ_main))

all: euler

euler: $(OBJ_main)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ euler

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)