#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#define radius 1

void generatecoords_l(double *, __uint128_t *);
void generatecoords_w(double *, uint64_t *);
int checkcircle(double *);
uint64_t wyhash64(uint64_t *);
uint64_t lehmer64(__uint128_t *);

int main(int argc, char **argv)
{
    struct timespec start, end;
    unsigned long long samples = 1000000L;
    int opt;
    char generator = 'l';
    while ((opt = getopt(argc, argv, ":n:hw")) != -1)
    {
        switch (opt)
        {
        case 'h':
            printf("options:\n-h\tfor help (display this message)");
            printf("\n-n\tto input number of coordinate pairs to approximate pi with.\n");
            printf("-w\tto generate coordinate pairs by wyhash method(will otherwise generate with lehmer's generator)\n");
            printf("will approximate pi with 1000000 coordinate pairs generated by lehmer's generator if no arguments are provided.\n");
            return 0;
        case 'n':
            samples = atoll(optarg);
            break;
        case 'w':
            generator = 'w';
            break;
        case '?':
            printf("unkown option: %s\n", optopt);
            return -1;
        case ':':
            printf("option %s requires a value\n", optopt);
            return -1;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (samples == 0)
    {
        printf("Invalid size. Goodbye.\n");
        return 0;
    }

    double allcoordinates[2];

    int i;
    unsigned long long counter = 0;
    // this is to be able to (among other things) put in more random gen methods for funsies later
    switch (generator)
    {
    case 'w':
        uint64_t seed = (uint64_t)((clock() + time(NULL)) * 2) + 1;
        {
            generatecoords_w(allcoordinates, &seed);
            counter += checkcircle(allcoordinates);
        }
    case 'l':
        // can be seeded to any odd number
        __uint128_t state = (__uint128_t)((clock() + time(NULL)) * 2) + 1;
        for (i = 0; i < samples; i++)
        {
            generatecoords_l(allcoordinates, &state);

            counter += checkcircle(allcoordinates);
        }
    }
    double result = (double)counter / samples;
    result *= 4;
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsedtime = (double)(end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);

    printf("Pi is roughly equal to %lf (sample size: %d). Elapsed time: About %lf seconds.\n", result, samples, elapsedtime);
    return 0;
}
// generate one set of coordinates between 0 and 1
void generatecoords_l(double *coords, __uint128_t *seed)
{
    int i;
    // create x and y coordinates between 0 and 1
    // ULONG_MAX is the max of wyhas64 on a 64 bit system
    for (i = 0; i < 2; i++)
    {
        coords[i] = (double)lehmer64(seed) / ULLONG_MAX;
    }
}
void generatecoords_w(double *coords, uint64_t *seed)
{
    int i;
    // create x and y coordinates between 0 and 1
    // ULONG_MAX is the max of wyhas64 on a 64 bit system
    for (i = 0; i < 2; i++)
    {
        coords[i] = (double)wyhash64(seed) / ULLONG_MAX;
    }
}
// 1 indicates inside circle
int checkcircle(double *coords)
{
    return !(coords[0] * coords[0] + coords[1] * coords[1] > radius);
}

// maximum return value:
// uint64_t max aka ULONG_MAX
// wyhash
uint64_t wyhash64(uint64_t *seed)
{
    (*seed) += 0x60bee2bee120fc15;
    __uint128_t tmp;
    tmp = (__uint128_t)(*seed) * 0xa3b195354a39b70d;
    uint64_t m1 = (tmp >> 64) ^ tmp;
    tmp = (__uint128_t)m1 * 0x1b03738712fad5c9;
    uint64_t m2 = (tmp >> 64) ^ tmp;
    return m2;
}

// lehmer64
// casting should be faster than bitshifting, it was when compiled with 0 optimization
// when compiled with more opt the compiler seems to give the same instruction, the times are identical.
uint64_t lehmer64(__uint128_t *state)
{
    (*state) *= 0xda942042e4dd58b5;
    return *(((uint64_t *)state) + 1);
}
