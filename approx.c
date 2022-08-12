#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#define radius 1

void generatecoords(float *);
int checkcircle(float *);

int main()
{
    srand(time(NULL));
    struct timespec start, end;
    char temp[21];
    //do high numbers at own risk, do not recommend
    printf("How many samples do you wish to use? (%u maximum).\n", UINT_MAX);
    fgets(temp, 21, stdin);
    unsigned int samples = atoi(temp);
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (samples == 0)
    {
        printf("Invalid size. Goodbye.\n");
        return 0;
    }
    
    float *allcoordinates = malloc(sizeof(float)*2);

    int i;
    unsigned long long counter = 0;

    for (i=0; i<samples; i++) 
    {
        generatecoords(allcoordinates);
        counter+=checkcircle(allcoordinates);
    }
    double result = (double) counter / samples;
    result*=4;
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsedtime = (double) (end.tv_sec-start.tv_sec)+ 1e-9*(end.tv_nsec-start.tv_nsec);

    printf("Pi is roughly equal to %lf (sample size: %d). Elapsed time: About %lf seconds.\n", result, samples, elapsedtime);
    free(allcoordinates);
    return 0;
}
//generate one set of coordinates between 0 and 1
void generatecoords(float * coords)
{
    int i;
    //create x and y coordinates between 0 and 1 (max value of rand is 1)
    for (i=0; i<2; i++) coords[i] = (float)1/RAND_MAX*rand();

}

//1 indicates inside circle
int checkcircle(float * coords)
{
//    printf("%lf, %lf\n",coords[0],coords[1]);
    if (coords[0]*coords[0] + coords[1] * coords[1] > radius) return 0;
    else return 1;
}