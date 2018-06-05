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
    int stride = floor( (float) strlen / (float) gsize);

    // MPI_Scatterv params
    if (rank == 0)
    {
        int *displs,*scounts;
        displs = (int *)malloc(gsize*sizeof(int));
        for (int i=0; i < gsize; ++i) displs[i] = i*stride;
    }

    int *scounts;
    scounts = (int *)malloc(gsize*sizeof(int));
    for (int i=0; i < gsize; ++i)
    {
        scounts[i] = stride;
    }
    int rest = strlen - stride * (gsize-1);
    if (rest != 0) scounts[gsize-1] = rest;
    
    char rbufs[scounts[rank]];

    // Distribute jobs
    MPI_Scatterv(str, scounts, displs, MPI_CHAR,  \
                      rbufs, scounts[rank], MPI_CHAR,       \
                     0, MPI_COMM_WORLD);

    reverse_str(rbufs, scounts[rank]);

    if (rank == 0)
    {
        int *r_displ;
        r_displ = (int*)malloc(gsize*sizeof(int));
        for(int i =0; i < gsize; i++)
        {
            r_displ[i]=strlen-i*stride-scounts[i] ;
        }

        strncpy(&str[r_displ[0]], rbufs, scounts[0]);
        for(int i = 1; i < gsize; i++)
        {
            char recv[scounts[i]];
            MPI_Recv(recv, scounts[i], MPI_CHAR, i, 16, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            strncpy(&str[r_displ[i]], recv, scounts[i]);
	}

    }
    else
    {
            MPI_Send(rbufs, scounts[rank], MPI_CHAR, 0, 16, MPI_COMM_WORLD);
    }

   free(displs);
   free(scounts);
   free(rev_displs);

    return;
}
