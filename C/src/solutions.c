#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "../include/solutions.h"


int problem_1() {
    int n = 1000;
    int counter = 0;
	for (int i = 0; i < n; i++) {
        if (i % 3 == 0 || i % 5 == 0) {
            counter += i; 
        }
	}
	return counter;
}

int problem_2() {
    int n = 4000000;
    long int *fib_memo = malloc(3 * sizeof(long int));
    fib_memo[0] = 1;
    fib_memo[1] = 2;
    fib_memo[2] = -1;
    long int sum = 2;
	while (fib_memo[2] < n) {
        fib_memo[2] = fib_memo[1] + fib_memo[0];
        fib_memo[0] = fib_memo[1];
        fib_memo[1] = fib_memo[2];
        if (fib_memo[2] % 2 == 0) {
            sum += fib_memo[2];
        }
    }
    return sum;
}

int problem_3() {
    long int n = 600851475143;
    int max_num = (int)sqrt(n) + 1;
    bool prime;
	for (int i = max_num; i > 3; i -= 2) {
        if (n % i == 0) {
            prime = true;
            for (int j = 3; j < (int)sqrt(i) + 1; j += 2) {
                if (i % j == 0) {
                    prime = false;
                    break;
                }
            }
            if (prime == true) {
                return i;
            } 
        }
	}
}

int problem_4() {
    int product;
    int tmp;
    int sum;
    int max_palindrome = 0;
	for (int i = 999; i > 100; i -= 1) {
        for (int j = 999; j > i; j -= 1) {
            product = i * j;
            tmp = product;
            sum = 0;
            while (tmp != 0) {
                sum = (sum * 10) + tmp % 10;
                tmp /= 10;
            }
            if (product == sum) {
                if (max_palindrome < product) {
                    max_palindrome = product;
                }
            }
        }
	}
    return max_palindrome;
}

int problem_5() {

    int size = 8;
    int primes[8] = {2,3,5,7,11,13,17,19};
    int a[8] = {0,0,0,0,0,0,0,0};
    long int result = 1;
    bool check = true;
    float limit = sqrt(20);

    for (int i = 0; i < size; i++) {
        a[i] = 1;
        if (check == true) {
            if (primes[i] <= limit) {
                a[i] = floor(log(20) / log(primes[i]));
            }
            else {
                check = false;
            }
        }
        result = result * pow(primes[i], a[i]);
    }
    return result;
}

int problem_6() {
    int n = 100;
    int squares_sum = 0;
    int sum_squared = 0;
    for (int i = 1; i <= n; i++) {
        squares_sum += pow(i, 2);
        sum_squared += i;
    }
    return pow(sum_squared, 2) - squares_sum;
}