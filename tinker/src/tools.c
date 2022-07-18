// -----------------------------------------------------------------------------
//                 Julian Melendez Tinker .arc parser  (2022)
// -----------------------------------------------------------------------------

#include "../include/tools.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Map mass_map[] = {
    {"H", 1.0078},
    {"C", 12.011},
    {"N", 14.007},
    {"O", 15.999},
};

Atom *create_atom() {
    Atom *atom = malloc(sizeof(Atom));
    atom->index = -1;
    strcpy(atom->name, "none");
    atom->pos[0] = 0.0;
    atom->pos[1] = 0.0;
    atom->pos[2] = 0.0;
    atom->type = -1;
    return atom;
}

Frame *create_frame(int num_atoms) {
    Frame *frame = malloc(sizeof(Frame));
    frame->num_atoms = num_atoms;
    frame->atoms = malloc(num_atoms * sizeof(Atom *));
    frame->box[0] = 0.0;
    frame->box[1] = 0.0;
    frame->box[2] = 0.0;
    for (int i = 0; i < num_atoms; i++) {
        frame->atoms[i] = *create_atom();
    }
    return frame;
}

Rdf *create_rdf(float min, float max, float step) {
    Rdf *rdf = malloc(sizeof(Rdf));
    int bins = (int)((max - min) / step);
    rdf->dr = step;
    rdf->r_max = max;
    rdf->num_bins = bins;
    rdf->g_r = malloc(bins * sizeof(float));
    rdf->n_r = malloc(bins * sizeof(float));
    return rdf;
}

int is_number(char *str) {
    if (*str == '\0') return 0;
    while (*str != '\0') {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

void split_atom(Atom *atom, char *line) {
    char *token;
    token = strtok(line, " ");  // Used to point the the next token
    atom->index = atoi(token);
    token = strtok(NULL, " ");
    strcpy(atom->name, token);
    token = strtok(NULL, " ");
    atom->pos[0] = atof(token);
    token = strtok(NULL, " ");
    atom->pos[1] = atof(token);
    token = strtok(NULL, " ");
    atom->pos[2] = atof(token);
    token = strtok(NULL, " ");
    atom->type = atof(token);
}

void split_box(Frame *fr, char *line) {
    char *token;
    token = strtok(line, " ");  // Used to point the the next token
    fr->box[0] = atof(token);
    token = strtok(NULL, " ");
    fr->box[1] = atof(token);
    token = strtok(NULL, " ");
    fr->box[2] = atof(token);
}

void update_frame(Frame *fr, FILE *f, long begin, int *sele) {
    char c;
    int num_lines = 0;
    int bytes = 0;
    int counter = 0;
    char *line = malloc(BUFFER);

    fseek(f, begin, SEEK_SET);  // Sets the start to begin reading the file
    while (bytes < num_bytes) {
        c = fgetc(f);
        bytes++;
        if (c != '\n') {
            line[counter] = c;
            counter++;
        } else {
            if (num_lines != 0) {
                if (num_lines == 1) {
                    split_box(fr, line);
                } else {
                    // ignores header lines fails for NVE
                    split_atom(&fr->atoms[num_lines - HEADER], line);
                }
            }
            // Clear the buffer of all the trash
            memset(line, '\0', BUFFER);
            num_lines++;
            counter = 0;
        }
    }
    free(line);
}

void print_atom(Atom *atom) {
    printf("%8d %4s %12.6f %12.6f %12.6f %4d\n", atom->index, atom->name,
           atom->pos[0], atom->pos[1], atom->pos[2], atom->type);
}

void print_frame(Frame *fr) {
    for (int i = 0; i < fr->num_atoms; i++) {
        print_atom(&fr->atoms[i]);
    }
}

void compute_com(Frame *fr, int frame) {
    bool found = false;
    double *com = calloc(DIM, sizeof(double));
    double total_mass = 0.0;
    for (int i = 0; i < fr->num_atoms; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(mass_map[j].key, fr->atoms[i].name) == 0) {
                for (int k = 0; k < DIM; k++) {
                    com[k] += mass_map[j].value * fr->atoms[i].pos[k];
                }
                total_mass += mass_map->value;
                found = true;
                break;
            }
        }
        if (found == false) {
            printf("No mass found for atom %s", fr->atoms[i].name);
            exit(1);
        }
    }
    printf("%8d %12.6f %12.6f %12.6f\n", frame, com[0] / total_mass,
           com[1] / total_mass, com[2] / total_mass);
}

double compute_mass(Frame *fr) {
    bool found = false;
    double total_mass = 0.0;
    for (int i = 0; i < fr->num_atoms; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(mass_map[j].key, fr->atoms[i].name) == 0) {
                total_mass += mass_map[j].value;
                found = true;
                break;
            }
        }
        if (found == false) {
            printf("No mass found for atom %s", fr->atoms[i].name);
            exit(1);
        }
    }
    return total_mass;
}

double *compute_distance(Frame *fr, int selected) {
    double ref[3] = {fr->atoms[selected].pos[0], fr->atoms[selected].pos[1],
                     fr->atoms[selected].pos[2]};
    double *distances = calloc(fr->num_atoms, sizeof(double));
    for (int i = 0; i < fr->num_atoms; i++) {
        for (int j = 0; j < DIM; j++) {
            double b = fr->box[j];
            double b_2 = b / 2.0;
            double other = fr->atoms[i].pos[j];
            double tmp = fmod(ref[j] - other + b_2, b) - b_2;
            distances[i] += pow(tmp, 2);
        }
        distances[i] = sqrt(distances[i]);
    }
    return distances;
}

void compute_rdf(Frame *fr, Rdf *rdf, int selected) {
    double *dists = compute_distance(fr, selected);
    for (int i = 0; i < fr->num_atoms; i++) {
        printf("%d %f\n", i, dists[i]);
    }
}
