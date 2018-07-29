#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// gcc -fopenmp -o bin/exercise3 src/exercise3.c -lgomp
// OMP_NUM_THREADS=7 ./bin/exercise3

int main(int argc, char *argv[]) {
	int ***A;
	int i, j, k, min, max;
	int gmin,gmax;
	
	struct timeval tv;
	gettimeofday(&tv,NULL);
	srand((int)tv.tv_usec);
	
	fprintf(stderr," allocating memory for A[][][]...");
	A = (int ***) malloc(3 * sizeof(int*));
	for(i = 0; i < 3; i++) {
		A[i] = (int **)malloc(1000*sizeof(int*));
		
		for(j = 0; j < 1000; j++) {
			A[i][j] = (int *) malloc(1000 * sizeof(int));
		}
	}
    fprintf(stderr,"done\n");
    
    fprintf(stderr," filling A[][][] with random values [-99:99]...");
    for(i = 1; i <=3 ; i++) {
    	for (j = 1; j<=1000 ; j++ ) {
    		for(k = 1; k <= 1000; k++) {
    			A[i-1][j-1][k-1] = (int)(100.0 - 200.0 * ((float)rand()/RAND_MAX));
    		}
    	}
    }
    fprintf(stderr,"done\n");
    
    min = A[0][0][0];
    max = A[0][0][0];
    
    gmin = min;
    gmax = max;
	
	/*
	 * omp parallel (directive):
	 * Start of parallel region.
	 * private (i,j,k) (clause):
	 * i, j and k are private to each thread, they are not initialized and they
	 * are not persistent (do not exist after parallel region).
	 * firstprivate (min,max) (clause):
	 * Like i, j and k but initialized with values before parallel region.
	 * shared (A,gmin,gmax) (clause):
	 * A, gmin and gmax are shared among all threads.
	 */
#pragma omp parallel private (i,j,k) firstprivate (min,max) shared (A,gmin,gmax)
	{
		for(i = 0; i < 3; i++) {
		/*
		 * for (work-sharing construct):
		 * Works only within a parallel region. Iterations of for-loop are
		 * executed in parallel.
		 * schedule (static) (clause):
		 * for-loop is devided into equally-sized chunks and assigned statically
		 * to threads.
		 * nowait:
		 * Threads do not synchronize at the end of the for-loop.
		 */
#pragma omp for schedule (static) nowait
			for(j = 0; j < 1000; j++) {
				for(k = 0; k < 1000; k++) {
					if(A[i][j][k] < min) {
						min = A[i][j][k];
					}
					if(A[i][j][k] > max) {
						max = A[i][j][k];
					}
				}
			}
		}

		/*
		 * critical (synchronisation construct):
		 * Works only within a parallel region. Is processed by all threads but
		 * only by one thread at a time.
		 */
#pragma omp critical
		{
			if(min < gmin) {
				gmin = min;
			}
			if(max > gmax) {
				gmax = max;
			}
		}
	/*
	 * End of parallel region.
	 */
	}

    fprintf(stderr," min = %d\n",gmin);
    fprintf(stderr," max = %d\n",gmax);

    /*
    * The following is an alternative (more elegant?) solution that makes use of OpenMP's reduction.
    * Here, we do not need to explicitly use a critical section in order to max/min-reduce thread-private minimum and maximum values.
    */
    gmin = A[0][0][0];
    gmax = A[0][0][0];

#pragma omp parallel private (i,j,k) firstprivate (min,max) shared (A,gmin,gmax)
	{
		for(i = 0; i < 3; i++) {
#pragma omp for schedule (static) reduction(min:gmin) reduction(max:gmax) 
			for(j = 0; j < 1000; j++) {
				for(k = 0; k < 1000; k++) {
					if(A[i][j][k] < gmin) {
						gmin = A[i][j][k];
					}
					if(A[i][j][k] > gmax) {
						gmax = A[i][j][k];
					}
				}
			}
		}
	}


    fprintf(stderr," min method 2 = %d\n",gmin);
    fprintf(stderr," max method 2 = %d\n",gmax);
}

