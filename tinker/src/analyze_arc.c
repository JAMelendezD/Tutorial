// -----------------------------------------------------------------------------
//                 Julian Melendez Tinker .arc parser  (2022)
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tools.h"

/*
 * Read the index files and outputs outputs it as a map struct
 */
Map *read_index(char *ndx, int *num_options) {
    FILE *f;
    char c;
    int groups = 0;

    // Counts the number of groups
    f = fopen(ndx, "r");
    while ((c = fgetc(f)) != EOF) {
        if (c == '[') {
            groups++;
        }
    }
    fclose(f);

    // Modifies external variable num_options with groups
    *num_options = groups;
    Map *options = malloc(groups * sizeof(Map));

    for (int i = 0; i < groups; i++) {
        options[i].key = malloc(BUFFER);
        options[i].value = -1;
    }

    // Reads file again and fills map struct with possible groups
    groups = -1;
    int counter = 0;
    char *tmp = malloc(BUFFER);

    f = fopen(ndx, "r");
    while ((c = fgetc(f)) != EOF) {
        if (c == '[') {
            memset(tmp, '\0', BUFFER);
            groups++;
        } else if (c == ']') {
            strcpy(options[groups].key, tmp);
            options[groups].value = groups;
        } else if (c == '\n') {
            memset(tmp, '\0', BUFFER);
            counter = 0;
        } else {
            tmp[counter] = c;
            counter++;
        }
    }
    free(tmp);
    fclose(f);
    return options;
}

int *get_indeces(char *ndx, int num, int *size) {
    FILE *f;
    int groups = -2;
    int count = 0;
    char *tmp = malloc(BUFFER);
    int *indeces = malloc(MAXINDEX * sizeof(int));

    f = fopen(ndx, "r");
    while (fscanf(f, "%s", tmp) != EOF) {
        if (is_number(tmp)) {
            indeces[count] = atoi(tmp);
            count++;
        } else {
            groups++;
            if (groups == num) {
                break;
            }
            count = 0;
        }
    }
    realloc(indeces, count * sizeof(int));
    *size = count;
    free(tmp);
    fclose(f);

    return indeces;
}

void print_options(Map *options, int num) {
    for (int i = 0; i < num; i++) {
        printf("%5d %30s\n", (int)options[i].value, options[i].key);
    }
    return;
}

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
 * Calculates the number of bytes in one frame based on the number of
 * atoms plus a header of 2 this doesnt work for NVE
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
    printf("Usage: %s -f <.arc or .xyz> -n <.ndx> -b <int> -e <int>\n", name);
    printf("  -f: Trajectory file in an arc format.\n");
    printf("  -n: Index file.\n");
    printf("  -b: First frame starts at 0.\n");
    printf("  -e: Last frame to analyze (inclusive).\n");
    exit(1);
}

int main(int argc, char **argv) {
    char *arc = malloc(BUFFER);  // arc file name
    char *ndx = malloc(BUFFER);  // index file name
    int *indeces;                // array of int for index selection
    FILE *arc_file;              // arc file
    Map *options;                // map number to index group name
    int num_options = 0;         // number of groups in index
    long b = 0;                  // first frame
    long e = 1;                  // last frame
    long frame_size;             // Number of bytes of a frame
    int selection = 0;           // Number of selected group
    int selection_size;          // Size of the selection

    if (argc != 9) {
        usage(argv[0]);
    }

    for (int iarg = 1; iarg < argc; iarg++) {
        if (strcmp(argv[iarg], "-f") == 0) {
            if (iarg + 1 == argc) {
                printf("Error: Missing problem number.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%s", arc) != 1) {
                printf("Error: Invalid problem number: %s\n", argv[iarg]);
                usage(argv[0]);
            }
        } else if (strcmp(argv[iarg], "-n") == 0) {
            if (iarg + 1 == argc) {
                fprintf(stderr, "Error: Missing index file.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%s", ndx) != 1) {
                fprintf(stderr, "Error: Invalid frame to start: %s\n",
                        argv[iarg]);
                usage(argv[0]);
            }
        } else if (strcmp(argv[iarg], "-b") == 0) {
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
        } else if (strcmp(argv[iarg], "-e") == 0) {
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

    options = read_index(ndx, &num_options);
    print_options(options, num_options);

    scanf("%d", &selection);

    if (selection < 0 || selection >= num_options) {
        printf("Invalid selection\n");
        exit(1);
    }

    indeces = get_indeces(ndx, selection, &selection_size);
    printf("# Number of atoms in selection: %d\n", selection_size);

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

    // // Creates a frame and loads it with the first frame
    Frame *arc_frame = create_frame(selection_size);
    update_frame(arc_frame, arc_file, b * frame_size, selection);

    print_frame(arc_frame);

    // // Computes the mass of the system based on the first frame
    // mass = compute_mass(arc_frame);
    // printf("# Mass of the system %f\n", mass);

    // // Main loop to do any analisis
    // // Center of mass example
    // for (int i = b; i <= e; i++) {
    //     update_frame(arc_frame, arc_file, i * frame_size,
    //     frame_size); compute_com(arc_frame, i);
    // }
    // fclose(arc_file);
    // free(arc_frame);

    // Main loop to do any analisis
    // Center of mass example
    // Rdf *na_rdf = create_rdf(1.0, arc_frame->box[0] / 2.0, 0.2);
    // int selected = 3;
    // for (int i = b; i <= e; i++) {
    //     update_frame(arc_frame, arc_file, i * frame_size,
    //     frame_size); compute_rdf(arc_frame, na_rdf, selected);
    // }
    // fclose(arc_file);
    // free(arc_frame);
}