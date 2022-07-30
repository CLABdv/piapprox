#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#define radius 1

double * generatecoords(void);
int checkcircle(double *);

int main()
{
    srand(time(NULL));
    time_t starttime;
    char temp[21];
    //do high numbers at own risk, do not recommend
    printf("How many samples do you wish to use? (%u maximum).\n", UINT_MAX);
    fgets(temp, 21, stdin);
    unsigned int samples = atoi(temp);
    time(&starttime);

    if (samples == 0)
    {
        printf("Invalid size. Goodbye.\n");
        return 0;
    }
    
    double * allcoordinates = malloc(sizeof(double)*2);

    int i;
    int counter = 0;

    for (i=0; i<samples; i++) 
    {
        allcoordinates=generatecoords();
        counter+=checkcircle(allcoordinates);
    }
    counter*=4;
    double result = (double) counter / samples;

    time_t elapsedtime = time(NULL)-starttime;

    printf("Pi is roughly equal to %lf (sample size: %d). Elapsed time: About %ld seconds.\n", result, samples, elapsedtime);
    free(allcoordinates);
    return 0;
}
//generate one set of coordinates between 0 and 1
double * generatecoords(void)
{
    double * coords = malloc(sizeof(double)*2);
    int i;
    //create x and y coordinates between 0 and 1 (max value of rand is 1)
    for (i=0; i<2; i++) coords[i] = (double)1/RAND_MAX*rand();

    return coords;
}

//1 indicates inside circle
int checkcircle(double * coords)
{
//    printf("%lf, %lf\n",coords[0],coords[1]);
    if (coords[0]*coords[0] + coords[1] * coords[1] > radius) return 0;
    else return 1;
}