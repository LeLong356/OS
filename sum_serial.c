#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
         return 1; 
    }
    long long n = atoll(argv[1]);
    if (n < 1 || n > 4000000000LL) { 
        return 1; 
    }

    unsigned long long sum = 0;
    for (long long i = 1; i <= n; i++) sum += i;

    printf("sum(%lld) = %llu\n", n, sum);
    return 0;
}
