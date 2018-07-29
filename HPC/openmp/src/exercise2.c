#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// gcc -fopenmp -o bin/exercise2 src/exercise2.c -lgomp
// OMP_NUM_THREADS=7 ./bin/exercise2

int main(int argc, char *argv[]) {
	int    i, N;
	double h, x, sum, PI;

    N = 100;
    h = 1.0/N;
    sum = 0.0;
    
    const int id1 = omp_get_thread_num();
    printf("Erstes Hello World thread %d\n", id1);
    
	/*
	 * omp parallel (directive):
	 * Start of parallel region.
	 * private (i,x) (clause):
	 * i and k are private to each thread, they are not initialized and they
	 * are not persistent (do not exist after parallel region).
	 * shared (N,h,sum) (clause):
	 * N, h and sum are shared among all threads.
	 */
#pragma omp parallel private (i,x) shared (N,h,sum)
	{
		const int id = omp_get_thread_num();
		printf("Hello World from thread %d\n", id);

		/*
		 * for (work-sharing construct):
		 * Works only within a parallel region. Iterations of for-loop are
		 * executed in parallel.
		 * reduction (+:sum) (clause):
		 * Performs the reduction operation by applying the + operator on sum.
		 * schedule (static) (clause):
		 * for-loop is devided into equally-sized chunks and assigned statically
		 * to threads.
		 * nowait:
		 * Threads do not synchronize at the end of the for-loop.
		 */
#pragma omp for reduction (+:sum) schedule (static) nowait
		for(i = 1; i <= N; i++) {
			x = h * (i - 0.5);
			sum += 4.0 / (1.0 + x*x);
		}
	}

    PI = h * sum;

    printf(" PI = %f\n", PI);
}

