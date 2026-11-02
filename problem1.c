#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
 
#define MOVIES 1683

int main(int argc , char *argv[]){
    if(argc != 3) {
        return 1;
    }

    int shmid = shmget(IPC_PRIVATE, 2 * MOVIES * sizeof(double), 0600 | IPC_CREAT);
    if(shmid < 0) {
        perror("shmget");
        return 1;
    }
    double (*avg)[MOVIES] = shmat(shmid,NULL,0);
    if (avg == (void *)-1) { perror("shmat"); return 1; }

    for( int c = 0 ; c < 2; c++){
        int pid;
        pid = fork();
        if(pid < 0){
            perror("fork");
            return 1;
        }
        else if(pid == 0){
            FILE *f = fopen(argv[c + 1], "r");
            if (!f) { 
                perror(argv[c + 1]); 
                exit(1); }
        static double sum[MOVIES];
        static int cnt[MOVIES];

        int u,m,r;
        long t;
            while(fscanf(f, "%d %d %d %ld",&u, &m, &r, &t) == 4){
                if(m > 0 && m < MOVIES) {
                    sum[m] += r;
                    cnt[m]++;
                }
            }
            for(m = 1; m < MOVIES; m++){
                avg[c][m] = cnt[m] ? sum[m] / cnt[m] : 0;
            }

            fclose(f);
            shmdt(avg);
            exit(0);
        }
        else{
        }  
    }
    wait(NULL);
    wait(NULL);
    FILE *out = fopen("result.txt", "w");
    if (!out) {
        perror("result.txt");
    } else {
        fprintf(out, "movie  %s  %s\n", argv[1], argv[2]);
        for (int m = 1; m < MOVIES; m++) {
            if (avg[0][m] || avg[1][m]) {
                fprintf(out, "%5d  %.4f  %.4f\n", m, avg[0][m], avg[1][m]);
            }
        }
        fclose(out);
    }
    if (shmdt(avg) == -1) {
        perror("shmdt");
        return 1;
    }
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        return 1;
    }
    return 0;
}