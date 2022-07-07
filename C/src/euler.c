// -----------------------------------------------------------------------------
//           Julian Melendez Solutions to the Euler Project   (2022)
// -----------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../include/solutions.h" 

#define N 8

const static struct {
    int index;
    int (*function)(void);
} problems[N] = {
  { 1, problem_1 },
  { 2, problem_2 },
  { 3, problem_3 },
  { 4, problem_4 },
  { 5, problem_5 },
  { 6, problem_6 },
  { 7, problem_7 },
  { 8, problem_8 },
};

void usage(char *name) {
    printf("Usage: %s --num <N>\n", name);
    exit(1);
}

void execute(int num, int (*f)()) {
    int solution = 0;
    clock_t start = clock();
    solution = f();
    printf("Solution to problem %2d is: %10d took %10.8f seconds\n", num, solution,
        ((double)clock() - start)/CLOCKS_PER_SEC);
}

int main(int argc, char **argv) {

    int num = 0;
    bool implemented = false;

    if (argc != 3) {
        usage(argv[0]);
    }

    for (int iarg = 1; iarg < argc; iarg++) {
        
        if (strcmp(argv[iarg], "--num") == 0) {
            if (iarg + 1 == argc) {
                printf("Error: Missing problem number.\n");
                usage(argv[0]);
            }
            iarg++;
            if (sscanf(argv[iarg], "%d", &num) != 1 || num < 1) {
                printf("Error: Invalid problem number: %s\n", argv[iarg]);
                usage(argv[0]);
            }
        }
        else {
            printf("Error: Unknown argument: %s\n", argv[iarg]);
            usage(argv[0]);
        }
    }

    for (int i = 0; i < N; i++) {
        if (problems[i].index == num) {
            execute(num, problems[i].function);
            implemented = true;
            break;
        }
    }

    if (implemented == false) {
        printf("Problem %2d not implemented\n", num);
    }
}