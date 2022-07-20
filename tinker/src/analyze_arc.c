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
    int groups = -1;
    char c;
    int tmp;
    int *indeces = malloc(0);

    f = fopen(ndx, "r");
    while ((c = fgetc(f)) != EOF) {
        if (c == '[') {
            if (groups == num) {
                break;
            } else {
                *size = 0;
                groups++;
                while ((c = fgetc(f)) != ']') {
                }
            }
        } else {
            if (groups == num) {
                indeces = realloc(indeces, ++(*size) * sizeof(int));
                fscanf(f, " %d", &tmp);
                indeces[*size - 1] = tmp;
            }
        }
    }
    (*size)--;
    fclose(f);
    return indeces;
}

void print_options(Map *options, int num) {
    printf("\n");
    for (int i = 0; i < num; i++) {
        printf("%5d %30s\n", (int)options[i].value, options[i].key);
    }
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
long get_frame_bytes(char *arc, int num_atoms) {
    FILE *f;
    int num_lines = 0;
    long bytes = 0;
    char c;

    f = fopen(arc, "r");
    while (num_lines < num_atoms + HEADER) {
        c = fgetc(f);
        bytes++;
        if (c == '\n') {
            num_lines++;
        }
    }
    fclose(f);
    return bytes;
}

/*
 * Calculates the number of bytes in one frame based on the number of
 * atoms plus a header of 2 this doesnt work for NVE
 */
long *get_selection_offsets(char *arc, int *selection, int num_atoms,
                            int selection_size) {
    FILE *f;
    int num_lines = 0;
    char c;
    long bytes = 0;
    long *byte_arr = calloc(selection_size, sizeof(long));

    f = fopen(arc, "r");
    while (num_lines < num_atoms + HEADER) {
        c = fgetc(f);
        bytes++;

        if (c == '\n') {
            for (int i = 0; i < selection_size; i++) {
                if ((num_lines - HEADER) == selection[i] - 2) {
                    byte_arr[i] = bytes;
                    break;
                }
            }
            num_lines++;
        }
    }
    fclose(f);
    return byte_arr;
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
    Map *options;                // map number to index group name
    FILE *arc_file;              // arc file pointer
    int num_options = 0;         // number of groups in index
    long b = 0;                  // first frame
    long e = 1e10;               // last frame
    long frame_size = 0;         // Number of bytes of a frame
    long *offsets;               // int array of offsets
    int selection = 0;           // Number of selected group
    int selection_size = 0;      // Size of the selection

    if (argc < 5) {
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
                printf("Error: Missing index file.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%s", ndx) != 1) {
                printf("Error: Invalid frame to start: %s\n", argv[iarg]);
                usage(argv[0]);
            }
        } else if (strcmp(argv[iarg], "-b") == 0) {
            if (iarg + 1 == argc) {
                printf("Error: Missing start frame.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%ld", &b) != 1 || b < 0) {
                printf("Error: Invalid frame to start: %s\n", argv[iarg]);
                usage(argv[0]);
            }
        } else if (strcmp(argv[iarg], "-e") == 0) {
            if (iarg + 1 == argc) {
                printf("Error: Missing end frame.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%ld", &e) != 1 || e < b) {
                printf("Error: Invalid frame to end: %s\n", argv[iarg]);
                usage(argv[0]);
            }
        } else {
            printf("Error: Unknown argument: %s\n", argv[iarg]);
            usage(argv[0]);
        }
    }

    // Reads index file to figure out available options
    options = read_index(ndx, &num_options);
    print_options(options, num_options);
    printf("\n# Select a group from the options: ");
    scanf("%3d", &selection);

    // Check if user input is a valid selection
    if (selection < 0 || selection >= num_options) {
        printf("Invalid selection\n");
        exit(1);
    }

    // Based on user input creates an array with the indeces
    indeces = get_indeces(ndx, selection, &selection_size);
    printf("\n# Number of atoms in selection: %d\n", selection_size);

    // Computes the number of atoms based on the first line
    int num_atoms = get_num_atoms(arc);
    printf("# Number of atoms: %d\n", num_atoms);

    // Computes the number of bytes for a frame from the begining up to
    // num atoms plus two extra lines as header
    frame_size = get_frame_bytes(arc, num_atoms);
    printf("# Number of bytes per frame: %ld\n", frame_size);

    // Creates byte offsets based one selection to be used with fseek
    offsets = get_selection_offsets(arc, indeces, num_atoms, selection_size);

    // Open an leaves arc file open based on argument
    arc_file = fopen(arc, "r");
    printf("# Arc file: %s\n", arc);

    // // Creates a frame
    Frame *arc_frame = create_frame(selection_size);

    // Main loop to do any analisis
    // Center of mass example
    for (int i = b; i <= e; i++) {
        update_frame(arc_frame, arc_file, i * frame_size, selection_size,
                     offsets);
        // print_frame(arc_frame);
        compute_com(arc_frame);
        //  compute_distance(arc_frame, 0);
    }
    fclose(arc_file);
    free(arc_frame);
}