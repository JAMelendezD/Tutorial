SRC = ../src
IDIR = ../include
CC = gcc
CFLAGS = -g -I$(IDIR) -Wall -O2

ODIR = ../obj
LDIR = ../lib

LIBS =  -lm

_DEPS = tools.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ_main = tools.o analyze_arc.o
OBJ_main = $(patsubst %,$(ODIR)/%,$(_OBJ_main))

all: analyze_arc

analyze_arc: $(OBJ_main)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ analyze_arc

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
