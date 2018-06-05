#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 
#include "helper.h"
#include <math.h>

void reverse(char *str, int strlen)
{
    int gsize, rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &gsize);
    int stride = floor( (float) strlen / (float) (gsize));
    int rest = strlen%gsize;

    // MPI_Scatterv params
//(i<strlen%np)
    int *displs,*scounts;
    displs = (int *)calloc(sizeof(int),gsize);
    scounts = (int *)calloc(sizeof(int),gsize);
    for (int i=0; i < gsize; ++i)
    {
        scounts[i] = stride + ( i < rest );
	if (i == 0)
		displs[0]=0;
	else
		displs[i] = displs[i-1] + scounts[i-1];   

    }
//    printf("rank %d, displs %d, scounts %d\n",rank,displs[rank],scounts[rank]);

    char* rbufs;//char rbufs[scounts[rank]];
    rbufs  = (char*)malloc(scounts[rank]*sizeof(char));

    // Distribute jobs
    MPI_Scatterv(str, scounts, displs, MPI_CHAR,  \
                      rbufs, scounts[rank], MPI_CHAR,       \
                     0, MPI_COMM_WORLD);
    // Reverse
    reverse_str(rbufs, scounts[rank]);

    // Collect results from processes
    if (rank == 0)
    {
        memcpy(str+strlen-scounts[0], rbufs, scounts[0]*sizeof(char));
        for(int i = 1; i < gsize; i++)
        {
	    char* recv  = (char*)malloc(scounts[i]*sizeof(char));
            //char recv[scounts[i]];
            MPI_Recv(recv, scounts[i], MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            memcpy(str+strlen-displs[i]-scounts[i], recv, scounts[i]*sizeof(char));
	}
    }
    else
    {
            MPI_Send(rbufs, scounts[rank], MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

   free(displs);
   free(scounts);
   return;
}
