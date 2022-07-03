// -----------------------------------------------------------------------------
// gameoflife.c : Conways game of life in C.
// -----------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/solutions.h" 

void usage(char *name) {
    fprintf(stderr, "Usage: %s --problem <N>\n", name);
    exit(1);
}

void execute(int num, int (*f)()) {
    int solution = 0;
    clock_t start = clock();
    solution = f();
    printf("Solution to problem%2d is: %10d took %10.8f seconds\n", num, solution,
        ((double)clock() - start)/CLOCKS_PER_SEC);
}

int main(int argc, char **argv) {

    int problem = 0;
    
    if (argc != 3) {
        usage(argv[0]);
    }

    for (int iarg = 1; iarg < argc; iarg++) {
        
        if (strcmp(argv[iarg], "--problem") == 0) {
            if (iarg + 1 == argc) {
                fprintf(stderr, "Error: Missing problem number.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%d", &problem) != 1 || problem < 1) {
                fprintf(stderr, "Error: Invalid problem number: %s\n",
                        argv[iarg]);
                usage(argv[0]);
            }
        }
        else {
            fprintf(stderr, "Error: Unknown argument: %s\n", argv[iarg]);
            usage(argv[0]);
        }
    }

    if (problem == 1) {
        execute(problem, problem_1);
    }
    else if (problem == 2) {
        execute(problem, problem_2);
    }
    else if (problem == 3) {
        execute(problem, problem_3);
    }
    else if (problem == 4) {
        execute(problem, problem_4);
    }
    else if (problem == 5) {
        execute(problem, problem_5);
    }
    else if (problem == 6) {
        execute(problem, problem_6);
    }
    return 0;
}