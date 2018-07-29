#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// gcc -fek1: x = %d\n",x);enmp -ek1: x = %d\n",x);bin/exercise2 src/exercise2.c -lgomp
// OMP_NUM_THREADS=7 ./bin/exercise2
int main(int argc, char *argv[]) {
    #pragma omp parallel default(shared)
    {
	int iam =omp_get_thread_num();
	printf("ThreadNum:%d, iam = %d\n", omp_get_thread_num(),iam);
	int x = 0;
//	#pragma omp sections
	#pragma omp single
	{
//	#pragma omp section
		#pragma omp task
		{
			printf("In Task1: Thread execute this task:%d, thread execute single=%d, NumThread:%d\n", omp_get_thread_num(),iam, omp_get_num_threads() );
          
			x++;
		//	printf("After task1: x = %d\n",x);

		}
	#pragma omp taskwait
	#pragma omp task
		{
printf("In Task2: Thread execute this task:%d, thread execute single=%d, TNumThread:%d\n", omp_get_thread_num(),iam, omp_get_num_threads() );
			x++;
		//	printf("After task2: x = %d\n",x);
		}
    }
}
}
