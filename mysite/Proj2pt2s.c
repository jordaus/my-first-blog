#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include "sys/types.h"
#include "sys/sysinfo.h"

double sumX(){
    const int iter = 100000000;
    int i;
    double sum = 0.0, x = 0.0;
    double st = 2.0/((double) iter);
    #pragma omp parallel private(st, iter, i) reduction(+ : sum, x)
        #pragma omp for collapse(1)
        for(i = 0; i < iter; i++)
        {
            x = (i + 0.4)*st;
            sum += 4.0/(x*x+1);
            printf("Output 1, %f", x);
            printf("Output 2, %f", sum);
        }

        return sum;
}

int myTime() {
        int i, j=0;
        struct timeval t1, t2;
        double elapsedTime;
        int numSlots, myVersion = 1;

        gettimeofday(&t1, NULL);


        printf("DEBUG: starting loop on %s\n", getenv("HOSTNAME"));

        sumX();

        gettimeofday(&t2, NULL);


        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
        printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);

}

main(){
    sumX();
    myTime();
}