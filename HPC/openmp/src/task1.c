#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// gcc -fek1: x = %d\n",x);enmp -ek1: x = %d\n",x);bin/exercise2 src/exercise2.c -lgomp
// OMP_NUM_THREADS=7 ./bin/exercise2
int main(int argc, char *argv[]) {
    #pragma omp parallel default (shared)
    {
printf("parallel region, threads number = %d\n",omp_get_num_threads());

	#pragma omp single
	{
printf("single region, threads number = %d\n",omp_get_num_threads());
		#pragma omp task
		printf("hello\n");
		#pragma omp task
                printf("hello again\n");
	}

    }
}

