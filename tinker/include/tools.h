#pragma once
#include <stdio.h>
#define BUFFER 1000
#define DIM 3
#define HEADER 2
#define AU_TO_GCC 1.6605402
#define min(a, b) (((a) < (b)) ? (a) : (b))

typedef struct Atom {
    int index;
    char name[5];
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

typedef struct Rdf {
    float r_min;
    float r_max;
    float dr;
    int num_bins;
    float *g_r;
    float *n_r;
} Rdf;

/*
 * Initializes an atom struct
 */
Atom *create_atom();

/*
 * Initializes frame struct and fills it with a given number of atoms
 */
Frame *create_frame(int num_atoms);

/*
 * Initializes rdf struct
 */
Rdf *create_rdf(float min, float max, float step);

/*
 * Uses strtok to split a atom line into the appropriate fields of the
 * Atom struct
 */
void split_atom(Atom *atom, char *line);

/*
 * Uses strtok to split a box line into the appropriate fields of the
 * Frame struct
 */
void split_box(Frame *fr, char *line);

/*
 * Uses seek to find figure out where to start reading the arc file.
 * Reads the atom in that frame and updates the frame struct.
 */
void update_frame(Frame *fr, FILE *f, long begin, long num_bytes);

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
void compute_com(Frame *fr, int frame);

/*
 * Computes distances of all atoms to a selected one for a given frame
 */
double *compute_distance(Frame *fr, int selected);

/*
 * Computes the mass of the system for a given frame
 */
double compute_mass(Frame *fr);

/*
 *
 */
void compute_rdf(Frame *fr, Rdf *rdf, int selected);