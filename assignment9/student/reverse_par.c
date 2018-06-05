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

    // MPI_Scatterv params
    int *displs,*scounts;
    displs = (int *)malloc(gsize*sizeof(int));
    scounts = (int *)malloc(gsize*sizeof(int));
    for (int i=0; i < gsize; ++i)
    {
	displs[i] = i*stride;   
        scounts[i] = stride;
    }
    int rest = strlen - stride * (gsize-1);
    if (rest != 0) scounts[gsize-1] = rest;

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
//	int *r_displ;
//	r_displ = (int*) malloc ( gsize * sizeof (int) );
//	for(int i =0; i < gsize; i++)
//        {
//            r_displ[i]=strlen-i*stride-scounts[i] ;
//        }
        memcpy(str+strlen-scounts[0], rbufs, scounts[0]*sizeof(char));
        for(int i = 1; i < gsize; i++)
        {
            char recv[scounts[i]];
            MPI_Recv(recv, scounts[i], MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            memcpy(str+strlen-i*stride-scounts[i], recv, scounts[i]*sizeof(char));
            //memcpy(&str[r_displ[i]], recv, scounts[i]*sizeof(char));
	}
        free(r_displ);
    }
    else
    {
            MPI_Send(rbufs, scounts[rank], MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

   free(displs);
   free(scounts);
   return;
}
