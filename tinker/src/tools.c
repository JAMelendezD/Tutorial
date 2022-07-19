// -----------------------------------------------------------------------------
//                 Julian Melendez Tinker .arc parser  (2022)
// -----------------------------------------------------------------------------

#include "../include/tools.h"

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Map mass_map[] = {
    {"H", 1.0078}, {"C", 12.011}, {"N", 14.007}, {"O", 15.999}, {"S", 32.065},
};

Atom *create_atom() {
    Atom *atom = malloc(sizeof(Atom));
    atom->index = -1;
    atom->name = malloc(5 * sizeof(char));
    atom->pos[0] = 0.0;
    atom->pos[1] = 0.0;
    atom->pos[2] = 0.0;
    atom->type = -1;
    return atom;
}

Frame *create_frame(int num) {
    Frame *frame = malloc(sizeof(Frame));
    frame->num_atoms = num;
    frame->atoms = malloc(num * sizeof(Atom));
    frame->box[0] = 0.0;
    frame->box[1] = 0.0;
    frame->box[2] = 0.0;
    for (int i = 0; i < num; i++) {
        // printf("%d\n", i);
        frame->atoms[i] = *create_atom();
    }
    return frame;
}

int is_number(char *str) {
    if (*str == '\0') return 0;
    while (*str != '\0') {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

void update_frame(Frame *fr, FILE *f, long begin, int selection_size,
                  long *offsets) {
    char c;
    int counter = 0;
    int num_lines = 0;
    char *tmp = malloc(BUFFER);

    fseek(f, begin, SEEK_SET);
    while (1) {
        c = fgetc(f);
        if (c == EOF) {
            exit(0);
        }
        tmp[counter] = c;
        counter++;
        if (c == '\n') {
            if (num_lines == HEADER - 1) {
                break;
            }
            counter = 0;
            memset(tmp, '\0', BUFFER);
            num_lines++;
        }
    }

    sscanf(tmp, "%lf %lf %lf", &fr->box[0], &fr->box[1], &fr->box[2]);

    counter = 0;
    memset(tmp, '\0', BUFFER);
    for (int i = 0; i < selection_size; i++) {
        fseek(f, begin + offsets[i], SEEK_SET);
        while ((c = fgetc(f)) != '\n') {
            if (c == EOF) {
                exit(0);
            }
            tmp[counter] = c;
            counter++;
        }
        sscanf(tmp, "%d %s %lf %lf %lf %d", &fr->atoms[i].index,
               fr->atoms[i].name, &fr->atoms[i].pos[0], &fr->atoms[i].pos[1],
               &fr->atoms[i].pos[2], &fr->atoms[i].type);
        counter = 0;
        memset(tmp, '\0', BUFFER);
    }
    free(tmp);
}

void print_atom(Atom *atom) {
    printf("%8d %4s %12.6f %12.6f %12.6f %4d\n", atom->index, atom->name,
           atom->pos[0], atom->pos[1], atom->pos[2], atom->type);
}

void print_frame(Frame *fr) {
    printf("    BOX: %12.6f %12.6f %12.6f\n", fr->box[0], fr->box[1],
           fr->box[2]);
    for (int i = 0; i < fr->num_atoms; i++) {
        print_atom(&fr->atoms[i]);
    }
}

void compute_com(Frame *fr) {
    bool found;
    double *com = calloc(DIM, sizeof(double));
    double total_mass = 0.0;
    for (int i = 0; i < fr->num_atoms; i++) {
        found = false;
        for (int j = 0; j < 5; j++) {
            if (*mass_map[j].key == fr->atoms[i].name[0]) {
                for (int k = 0; k < DIM; k++) {
                    com[k] += mass_map[j].value * fr->atoms[i].pos[k];
                }
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
    printf("%12.6f %12.6f %12.6f\n", com[0] / total_mass, com[1] / total_mass,
           com[2] / total_mass);
}

void compute_distance(Frame *fr, int selected) {
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
        printf("%f\n", distances[i]);
    }
}