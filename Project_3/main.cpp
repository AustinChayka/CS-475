#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <omp.h>
#include <stdlib.h>

unsigned int    seed =                         123;

const float     GRAIN_GROWS_PER_MONTH =		    9.0;
const float     ONE_DEER_EATS_PER_MONTH =		1.0;

const float     AVG_PRECIP_PER_MONTH =		    7.0;	// average
const float     AMP_PRECIP_PER_MONTH =		    6.0;	// plus or minus
const float     RANDOM_PRECIP =			        2.0;	// plus or minus noise

const float     AVG_TEMP =				        60.0;	// average
const float     AMP_TEMP =				        20.0;	// plus or minus
const float     RANDOM_TEMP =			        10.0;	// plus or minus noise

const float     MIDTEMP =				        40.0;
const float     MIDPRECIP =				        10.0;

int	            NowYear;		// 2021 - 2026
int	            NowMonth;		// 0 - 11

float	        NowPrecip;		// inches of rain per month
float	        NowTemp;		// temperature this month
float	        NowHeight;		// grain height in inches
int	            NowNumDeer;		// number of deer in the current population
float           NowFire;


void Deer();
void Grain();
void Watcher();
void MyAgent();

float SQR( float x ) {
    return x*x;
}

float Ranf( unsigned int *seedp,  float low, float high ) {
        //srand(seed);
        float r = (float) rand();              // 0 - RAND_MAX
        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int Ranf( unsigned int *seedp, int ilow, int ihigh ) {
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;
        return (int)(  Ranf(seedp, low,high) );
}

int main(int argc, char** argv) {

    srand(seed);

    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
        NowPrecip = 0.;
    
    // starting date and time:
    NowMonth =    0;
    NowYear  = 2021;

    // starting state (feel free to change this if you want):
    NowNumDeer = 1;
    NowHeight =  1.;

    NowFire = 0.;

    omp_set_num_threads( 4 );	// same as # of sections
    #pragma omp parallel sections 
    {

        #pragma omp section 
        {
            Deer( );
        }

        #pragma omp section 
        {
            Grain( );
        }

        #pragma omp section 
        {
            Watcher( );
        }

        #pragma omp section 
        {
            MyAgent( );	// your own
        }
    }       // implied barrier -- all functions must return in order
        // to allow any of them to get past here

    return 0;

}

void Deer() {

    while( NowYear < 2027 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int nextNumDeer = NowNumDeer;
        int carryingCapacity = (int)( NowHeight );
        if( nextNumDeer < carryingCapacity )
            nextNumDeer++;
        else if( nextNumDeer > carryingCapacity )
            nextNumDeer--;

        if( nextNumDeer < 0 )
            nextNumDeer = 0;

        // DoneComputing barrier:
        #pragma omp barrier

        NowNumDeer = nextNumDeer;
        
        // DoneAssigning barrier:
        #pragma omp barrier
        

        // DonePrinting barrier:
        #pragma omp barrier
        
    }

}

void Grain() {

    while( NowYear < 2027 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:

        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );

        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

        float nextHeight = NowHeight;
        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        if(NowFire > 0) nextHeight -= 2;

        if( nextHeight < 0. ) nextHeight = 0.;

        // DoneComputing barrier:
        #pragma omp barrier

        NowHeight = nextHeight;
        
        // DoneAssigning barrier:
        #pragma omp barrier
        

        // DonePrinting barrier:
        #pragma omp barrier
        
    }

}

void Watcher() {

    while( NowYear < 2027 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        
        // DoneComputing barrier:
        #pragma omp barrier
        
        // DoneAssigning barrier:
        #pragma omp barrier
        
        printf("Month: %i\tYear: %i\tTemp: %f\t\tPercip: %f\tHeight: %f\tNumDeer: %i\tFire: %f\n", NowMonth, NowYear, NowTemp, NowPrecip, NowHeight, NowNumDeer, NowFire);

        NowMonth++;
        if(NowMonth >= 12) {
            NowMonth = 0;
            NowYear++;
        }

        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
            NowPrecip = 0.;

        // DonePrinting barrier:
        #pragma omp barrier
        
    }

}

void MyAgent() {

    while( NowYear < 2027 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        float nextFire = NowFire;
        if(NowFire == 0) {
            if(Ranf(&seed, 0., NowTemp) > 70) nextFire = 1;
        } else if(NowHeight > 0) {
            nextFire += NowHeight / 2;
        } else nextFire = nextFire * .75;

        if(NowPrecip > 5) nextFire -= NowPrecip / 10;
        
        if(nextFire < 0) nextFire = 0;

        // DoneComputing barrier:
        #pragma omp barrier
        
        NowFire = nextFire;

        // DoneAssigning barrier:
        #pragma omp barrier

        // DonePrinting barrier:
        #pragma omp barrier
        
    }

}