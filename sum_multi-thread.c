#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct job_t {
    long long start;  
    long long end;   
    long long result; 
};

void *sum(void *input) {
    struct job_t *j = (struct job_t *)input;
    long long s = 0;
    long long i;

    for (i = j->start; i <= j->end; i++) {
        s = s + i;
    }
    j->result = s;
    return NULL;
}

int main(int argc, char *argv[]) {
    struct job_t job[100];   
    pthread_t tid[100];     
    int k, i;
    long long n, chunk, sum1;

    if (argc != 3) {
        return 1;
    }
    k = atoi(argv[1]);     
    n = atoll(argv[2]);    
    if (k < 1 || k > 100 || n < 1) {
        return 1;
    }

    chunk = n / k;
    for (i = 0; i < k; i++) {
        job[i].start = i * chunk + 1;
        if (i == k - 1) {
            job[i].end = n;        
        } else {
            job[i].end = (i + 1) * chunk;
        }
        pthread_create(&tid[i], NULL, sum, (void *)&job[i]);
    }

    sum1 = 0;
    for (i = 0; i < k; i++) {
        pthread_join(tid[i], NULL);
        sum1 = sum1 + job[i].result;
    }

    printf("sum(%lld) = %lld\n", n, sum1);
    return 0;
}
