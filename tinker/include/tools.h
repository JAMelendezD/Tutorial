#pragma once
#include <stdio.h>
#define BUFFER 1000
#define DIM 3
#define HEADER 2
#define MAXINDEX 10000000
#define AU_TO_GCC 1.6605402
#define min(a, b) (((a) < (b)) ? (a) : (b))

typedef struct Atom {
    int index;
    char *name;
    double pos[3];
    int type;
} Atom;

typedef struct Frame {
    Atom *atoms;
    int num_atoms;
    double box[3];
} Frame;

typedef struct Map {
    char *key;
    double value;
} Map;

/*
 * Initializes an atom struct
 */
Atom *create_atom();

/*
 * Initializes frame struct and fills it with a given number of atoms
 */
Frame *create_frame(int num_atoms);

/*
 * Checks if all the characters in a string are integers
 */
int is_number(char *str);

/*
 * Uses seek to find figure out where to start reading the arc file.
 * Reads the atom in that frame and updates the frame struct.
 */
void update_frame(Frame *fr, FILE *f, long begin, int selection_size,
                  long *offsets);

/*
 * Gets the fields from the atom struct and prints them
 */
void print_atom(Atom *atom);

/*
 * Loops over all atoms in frame and uses print atom
 */
void print_frame(Frame *fr);

/*
 * Computes the center of mass of the system for a given frame
 */
void compute_com(Frame *fr);

/*
 * Computes distances of all atoms to a selected one for a given frame
 */
void compute_distance(Frame *fr, int selected);