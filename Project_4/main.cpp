#include "simd.cpp"
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>

int SIZE;

float Ranf( float low, float high ) {

    float r = (float) rand();               // 0 - RAND_MAX
    float t = r  /  (float) RAND_MAX;       // 0. - 1.

    return   low  +  t * ( high - low );

}

int main(int argc, char** argv) {
    
    if(argc >= 1) SIZE = atoi(argv[1]);
	else {
		printf("Array size not defined on on command line.");
		return 1;
    }

    #ifndef _OPENMP
        fprintf( stderr, "No OpenMP support!\n" );
        return 1;
    #endif

    float *a = new float[SIZE];
    float *b = new float[SIZE];

    for(int i = 0; i < SIZE; i++) {

        a[i] = Ranf(0, 100);
        b[i] = Ranf(0, 100);

    }

    float sumNonSIMD = 0, sumSIMD;
    double nonSIMDperf, SIMDperf, time0, time1;

    time0 = omp_get_wtime();
    for(int i = 0; i < SIZE; i++) sumNonSIMD += a[i] * b[i];
    time1 = omp_get_wtime();
    nonSIMDperf = double(SIZE) / (time1 - time0) / 1000000.;

    time0 = omp_get_wtime();
    sumSIMD = SimdMulSum(a, b, SIZE);
    time1 = omp_get_wtime();
    SIMDperf = double(SIZE) / (time1 - time0) / 1000000.;

    printf("nonSIMD sum: %.3f\tnonSIMD perf: %.3lf\tSIMD sum: %.3f\tSIMD perf: %.3lf\tSpeedup: %.3lf", sumNonSIMD, nonSIMDperf, sumSIMD , SIMDperf, (double) SIMDperf / nonSIMDperf);

    return 0;

}