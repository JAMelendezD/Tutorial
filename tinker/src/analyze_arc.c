// -----------------------------------------------------------------------------
//                 Julian Melendez Tinker .arc parser  (2022)
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tools.h"

/*
 * Gets the number of atoms from the first line of the arc file
 */
int get_num_atoms(char *arc) {
    FILE *f;
    f = fopen(arc, "r");
    int n = 0;
    char *tmp = malloc(BUFFER);
    fscanf(f, "%d %[^\n]", &n, tmp);
    fclose(f);
    free(tmp);
    return n;
}

/*
 * Calculates the number of bytes in one frame based on the number of atoms
 * plus a header of 2 this doesnt work for NVE
 */
int get_frame_bytes(char *arc, int num_atoms) {
    FILE *f;
    int num_lines = 0;
    int bytes = 0;
    char c;
    f = fopen(arc, "r");
    while (num_lines < num_atoms + HEADER) {
        c = fgetc(f);
        bytes++;
        if (c == '\n') {
            num_lines++;
        }
    }
    return bytes;
}

void usage(char *name) {
    printf("Usage: %s --arc <trajectory.arc> --b <0> --e <500>\n", name);
    printf("  --arc: Trajectory file in an arc format.\n");
    printf("    --b: First frame starts at 0.\n");
    printf("    --e: Last frame to analyze (inclusive).\n");
    exit(1);
}

int main(int argc, char **argv) {
    char *arc = malloc(BUFFER);
    FILE *arc_file;
    long b = 0;
    long e = 1;
    long frame_size;
    double mass;

    if (argc != 7) {
        usage(argv[0]);
    }

    for (int iarg = 1; iarg < argc; iarg++) {
        if (strcmp(argv[iarg], "--arc") == 0) {
            if (iarg + 1 == argc) {
                printf("Error: Missing problem number.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%s", arc) != 1) {
                printf("Error: Invalid problem number: %s\n", argv[iarg]);
                usage(argv[0]);
            }
        } else if (strcmp(argv[iarg], "--b") == 0) {
            if (iarg + 1 == argc) {
                fprintf(stderr, "Error: Missing start frame.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%ld", &b) != 1 || b < 0) {
                fprintf(stderr, "Error: Invalid frame to start: %s\n",
                        argv[iarg]);
                usage(argv[0]);
            }
        } else if (strcmp(argv[iarg], "--e") == 0) {
            if (iarg + 1 == argc) {
                fprintf(stderr, "Error: Missing end frame.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%ld", &e) != 1 || e < b) {
                fprintf(stderr, "Error: Invalid frame to end: %s\n",
                        argv[iarg]);
                usage(argv[0]);
            }
        } else {
            printf("Error: Unknown argument: %s\n", argv[iarg]);
            usage(argv[0]);
        }
    }

    // Open an leaves arc file open based on argument
    arc_file = fopen(arc, "r");
    printf("# Arc file: %s\n", arc);

    // Computes the number of atoms based on the first line
    int num_atoms = get_num_atoms(arc);
    printf("# Number of atoms: %d\n", num_atoms);

    // Computes the number of bytes for a frame from the begining up to
    // num atoms plus two extra lines as header
    frame_size = get_frame_bytes(arc, num_atoms);
    printf("# Number of bytes per frame: %ld\n", frame_size);

    // Creates a frame and loads it with the first frame
    Frame *arc_frame = create_frame(num_atoms);
    update_frame(arc_frame, arc_file, b * frame_size, frame_size);

    // print_frame(arc_frame);

    // Computes the mass of the system based on the first frame
    mass = compute_mass(arc_frame);
    printf("# Mass of the system %f\n", mass);

    // Main loop to do any analisis
    // Center of mass example
    for (int i = b; i <= e; i++) {
        update_frame(arc_frame, arc_file, i * frame_size, frame_size);
        compute_com(arc_frame, i);
    }
    fclose(arc_file);
    free(arc_frame);

    // Main loop to do any analisis
    // Center of mass example
    // Rdf *na_rdf = create_rdf(1.0, arc_frame->box[0] / 2.0, 0.2);
    // int selected = 3;
    // for (int i = b; i <= e; i++) {
    //     update_frame(arc_frame, arc_file, i * frame_size, frame_size);
    //     compute_rdf(arc_frame, na_rdf, selected);
    // }
    // fclose(arc_file);
    // free(arc_frame);
}