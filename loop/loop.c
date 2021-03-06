/*
 * loop.c:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hpctimer.h"

int DEBUG = 1;

enum { n = 16 * 1024 * 1024 };
int *v;

double repeat(int * v, int DEPTH) {
    
    int *temp_data;
    
    if ( (temp_data = malloc(sizeof(*temp_data) * DEPTH)) == NULL) {
		fprintf(stderr, "No enough memory\n");
    	exit(EXIT_FAILURE);
	}
	
    int i, j, sum = 0;
    double t;
    
    for(i=0; i<DEPTH; i++)
        temp_data[i] = 0;
    
    t = hpctimer_wtime();
    
    for (i = 0; i < n; i+=DEPTH) 
	    for(j=0;j<DEPTH;j++)
	        temp_data[j] += v[i];
    
    for(i = 0; i < DEPTH ; i++)
        sum += temp_data[i]; // Count summ
    
    t = hpctimer_wtime() - t;
    
    if(DEBUG) {
        printf("Sum = %d   ", sum);
        printf("Elapsed time: %.6f sec.\n", t);
    }
    free(temp_data);
    return t;
}

int main(int argc, char *argv[])
{
    int DEPTH[5] = {1, 2, 4, 8, 16};
    int i, j, deep_it, qwe = 10;
    
    double avr;
    double result[5] = {0};
    
	if ( (v = malloc(sizeof(*v) * n)) == NULL) {
		fprintf(stderr, "No enough memory\n");
    	exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < n; i++) {
		v[i] = 1;
	}
	
	for(j=0;j<5;j++) {
	    avr = 0;
	    for(deep_it = 0; deep_it <= qwe; deep_it++)
	        avr += repeat(v,DEPTH[j]);

        if(DEBUG)printf("%d - AVR: %.6f\n", DEPTH[j], avr/qwe);
        result[j] = avr/qwe;
	}
    
	free(v);
	
	for(i=0; i<5; i++)
	    printf("DEPTH: %d -> %.6f sec. | SpeedUp: %.6f\n", DEPTH[i], result[i], result[0]/result[i]);
	return 0;
}
