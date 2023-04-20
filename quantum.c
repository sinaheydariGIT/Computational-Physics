
//quantum Monte Carlo

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// define function prototypes
void initialize();
void walk();
void data();

// Constants
#define N0 50	 	//initial walkers
#define mcs 500	//MC steps
#define MAXX 200
#define maxpsi 100

double ds=0.1;  //random walk step
double dt=0.01; //time step 

int N;		 //# of walkers
double Vref; //ref energy

double x[MAXX+1];// position of each random walker
double psi[maxpsi+1]; // histogram

int main() {
	int step;
	FILE *fp1;
	double psiSum = 0.0;

  srand((unsigned)time((long *)0)); 

	initialize();
	for(step=1; step<=0.4*mcs; step++){
		walk();		 // thermalization
	}

	

	for (int i=1; i<=maxpsi; i++){
		psiSum += psi[i]*psi[i]*0.2;
	}
	psiSum = sqrt(psiSum);

	fp1 = fopen("qmcHistogram.txt", "w");
	for (int i=1; i<=maxpsi; i++){
		psi[i] = psi[i]/psiSum;
		fprintf(fp2, "%d %f \n", i, psi[i]);
	}
	fclose(fp2);

 	return 0;
}

// Function definitions
void initialize() {
	int i;
	N = N0;

	// Assign position to N0 random walkers
	for (i=1; i<=N; i++)
		x[i] = -1+2*rand()/(double) RAND_MAX ; /

	// Calculate reference energy
	for (i=1; i<=N; i++)
		Vref += pow(x[i],2)/2; 
	Vref = Vref/N;
}

void walk() {
	int i;
	int Nin = N;
	double Vavg, dV, Vsum=0.0;

	for (i=Nin; i>=1; i--) { 
	// Random Walk as in diffuse.c
		if(rand()%2==0) x[i] += ds;
		else x[i] -= ds;

		dV = pow(x[i],2)/2 - Vref;
		if (dV<0.0)
			if (rand()/(double)RAND_MAX < -dV*dt){
				N++;
				x[N] = x[i];
				Vsum += 2*pow(x[i],2)/2;} 
			else
				Vsum += pow(x[i],2)/2;
		else
			if ( rand()/(double)RAND_MAX < dV*dt){
				x[i] = x[N];
				N--;}
			else
				Vsum += pow(x[i],2)/2;
	}

	Vavg = Vsum/N;
	Vref = Vavg - (N-N0)/(double)(N0*dt);
}

void data(){
	double xshift, binsize;
	int bin;

	binsize = 2*ds;
	xshift = binsize*maxpsi*0.5;
	for(int i=1; i<=N; i++){
		bin = (x[i] + xshift + 0.5*binsize)/binsize;
		psi[(int)bin] += 1;
	}
}