#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <linux/time.h>


int prime_check(int k)
{
    int i;

    if (k < 2) {
        return 0;
    }

    if (k == 2) {
        return 1;
    }

    if (k % 2 == 0) {
        return 0;
    }

    // Only check odd divisors up to sqrt(k)
    for (i = 3; i <= (int)sqrt((double)k); i += 2) {
        if (k % i == 0) {
            return 0;
        }
    }

    return 1;
}


// Task 1 - Serial Code - Finding Prime Numbers

/*
Requirements:

- User inputs integer n
- Outputs a list of sorted prime numbers

- std output for n < 100
- txt file for n > 100
*/



int main(int argc, char *argv[]) {

    struct timespec start, end, startComp, endComp;
    double time_taken;

    // Get current clock time (overall start)
    clock_gettime(CLOCK_MONOTONIC, &start);

    // check command line requirements
    if (argc != 1) {
        printf("Usage: integer\n");
        return 1;
    }

    // take user input
    long long number;
    printf("Input integer: \n");
    scanf("%lld", &number);

    if (number <= 2) {
        printf("There are no prime numbers strictly less than %lld.\n", number);
        return 0;
    }

    // Dynamically allocate storage for the primes found
    int capacity = 1024;
    int count = 0;
    long long i;
    int *primes = (int *)malloc(capacity * sizeof(int));

    if (primes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("Compute\n");

    // Get current clock time (computation start)
    clock_gettime(CLOCK_MONOTONIC, &startComp);

    // Search for primes strictly less than 'number'
    for (i = 2; i < number; i++) {
        if (prime_check((int)i)) {

            // Grow storage if needed
            if (count == capacity) {
                capacity *= 2;
                primes = (int *)realloc(primes, capacity * sizeof(int));
                if (primes == NULL) {
                    fprintf(stderr, "Memory reallocation failed.\n");
                    return 1;
                }
            }

            primes[count] = (int)i;
            count++;
        }
    }

    // Get current clock time (computation end)
    clock_gettime(CLOCK_MONOTONIC, &endComp);
    time_taken = (endComp.tv_sec - startComp.tv_sec) * 1e9;
    time_taken = (time_taken + (endComp.tv_nsec - startComp.tv_nsec)) * 1e-9;
    printf("Prime search complete - Computational time only(s): %lf\n", time_taken);

    // Output: stdout for small n, file for larger n
    if (number < 100) {
        printf("\nPrime numbers less than %lld:\n", number);
        for (i = 0; i < count; i++) {
            printf("%d", primes[i]);
            if (i < count - 1) {
                printf(", ");
            }
        }
        printf("\n");
    } else {
        FILE *file = fopen("Primes.txt", "w");
        if (file == NULL) {
            fprintf(stderr, "Could not open Primes.txt for writing.\n");
            free(primes);
            return 1;
        }

        fprintf(file, "Prime numbers less than %lld:\n", number);
        for (i = 0; i < count; i++) {
            fprintf(file, "%d", primes[i]);
            if (i < count - 1) {
                fprintf(file, ", ");
            }
        }
        fprintf(file, "\n");

        fclose(file);
        printf("\nPrime numbers have been written to Primes.txt\n");
    }

    printf("Number of primes found: %d\n", count);

    free(primes);

    // Get current clock time (overall end)
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    printf("Overall time (Including input and output)(s): %lf\n", time_taken);

    return 0;
}