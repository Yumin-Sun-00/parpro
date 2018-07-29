#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// gcc -fek1: x = %d\n",x);enmp -ek1: x = %d\n",x);bin/exercise2 src/exercise2.c -lgomp
// OMP_NUM_THREADS=7 ./bin/exercise2
int main(int argc, char *argv[]){	
    int i = 3;
    #pragma omp parallel for
	for(int j= 0; j<1000;j++)
	{i = i+1;
        //printf("i= %d\n", i);
}
 printf("Final value i = %d\n",i);
}

