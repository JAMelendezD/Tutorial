#include "../include/tools.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Map mass_map[] = {
    {"C", 12.011},
    {"N", 14.007},
    {"O", 15.999},
    {"H", 1.0078},
};

Atom *create_atom() {
    Atom *atom = malloc(sizeof(Atom));
    atom->index = -1;
    strcpy(atom->name, "none");
    atom->x = 0.0;
    atom->y = 0.0;
    atom->z = 0.0;
    atom->type = -1;
    return atom;
}

Frame *create_frame(int num_atoms) {
    Frame *frame = malloc(sizeof(Frame));
    frame->num_atoms = num_atoms;
    frame->atoms = malloc(num_atoms * sizeof(Atom *));
    frame->box_x = 0.0;
    frame->box_y = 0.0;
    frame->box_z = 0.0;
    for (int i = 0; i < num_atoms; i++) {
        frame->atoms[i] = *create_atom();
    }
    return frame;
}

void split_atom(Atom *atom, char *line) {
    char *token;
    token = strtok(line, " ");  // Used to point the the next token
    atom->index = atoi(token);
    token = strtok(NULL, " ");
    strcpy(atom->name, token);
    token = strtok(NULL, " ");
    atom->x = atof(token);
    token = strtok(NULL, " ");
    atom->y = atof(token);
    token = strtok(NULL, " ");
    atom->z = atof(token);
    token = strtok(NULL, " ");
    atom->type = atof(token);
}

void split_box(Frame *fr, char *line) {
    char *token;
    token = strtok(line, " ");  // Used to point the the next token
    fr->box_x = atof(token);
    token = strtok(NULL, " ");
    fr->box_y = atof(token);
    token = strtok(NULL, " ");
    fr->box_z = atof(token);
}

void update_frame(Frame *fr, FILE *f, long begin, long num_bytes) {
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
           atom->x, atom->y, atom->z, atom->type);
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
                com[0] += mass_map[j].value * fr->atoms[i].x;
                com[1] += mass_map[j].value * fr->atoms[i].y;
                com[2] += mass_map[j].value * fr->atoms[i].z;
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
    com[0] /= total_mass;
    com[1] /= total_mass;
    com[2] /= total_mass;
    printf("%8d %12.6f %12.6f %12.6f\n", frame, com[0], com[1], com[2]);
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