#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#define N	0.70

#ifndef NUMTRIES
#define NUMTRIES 1
#endif

int NUMT;
int NUMNODES;

float Height( int, int );	// function prototype

int main( int argc, char *argv[ ] ) {

    #ifndef _OPENMP
	    fprintf( stderr, "No OpenMP support!\n" );
	    return 1;
    #endif

    if(argc >= 2) NUMT = atoi(argv[1]);
	else {
		printf("Num threads not defined on on command line.");
		return 1;
	}

	if(argc >= 3) NUMNODES = atoi(argv[2]);
	else {
		printf("Num nodes not defined on on command line.");
		return 1;
	}
	
    float volume;
    double maxPerformance = 0., bestTime = DBL_MAX;

	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)

	float fullTileArea = (	( ( XMAX - XMIN ) / (float)(NUMNODES-1) ) *
							( ( YMAX - YMIN ) / (float)(NUMNODES-1) ) );

    omp_set_num_threads(NUMT);

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

    for( int tries = 0; tries < NUMTRIES; tries++ ) {

		volume = 0;
        double time0 = omp_get_wtime( );

        #pragma omp parallel for default(none), shared(maxPerformance, NUMNODES, fullTileArea), reduction(+:volume)
        for(int i = 0; i < NUMNODES*NUMNODES; i++ ) {

            int iu = i % NUMNODES;
            int iv = i / NUMNODES;
            float z = Height( iu, iv );
            
            float t = z * fullTileArea;
			if(iu == 0 || iu == NUMNODES - 1) {
				if(iv == 0 || NUMNODES - 1) t *= .25;
				else t *= .5;
			} else if(iv == 0 || iv == NUMNODES - 1) t *= .5;
            volume += t;

        } 

		volume *= 2;

        double time1 = omp_get_wtime( );
		double time = (time1 - time0);
        double megaHeightsPerSecond = (double)(NUMNODES*NUMNODES) / time / 1000000.;
        if(megaHeightsPerSecond > maxPerformance ) {
			maxPerformance = megaHeightsPerSecond;
			bestTime = time;
		}

    }

    printf("cores: %i\t|\tnodes: %i\t|\tvolume: %f\t|\tmegaHeights/sec: %lf\t|\ttime: %lf", NUMT, NUMNODES, volume, maxPerformance, bestTime);

    return 0;
    
}

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(float)N );
	return height;
}