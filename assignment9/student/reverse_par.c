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
    int stride = ceil(strlen / (gsize));

    // MPI_Scatterv params
    int ROOT = 0;
    MPI_Datatype sendtype = MPI_CHAR;
    MPI_Datatype recvtype = MPI_CHAR;
    
    int *displs, *scounts; 
    displs = (int *)malloc(gsize*sizeof(int)); 
    scounts = (int *)malloc(gsize*sizeof(int));
    for (int i=0; i < gsize; ++i) { 
        displs[i] = i*stride; 
        scounts[i] = stride; 
    } 
    int rest = strlen - stride * (gsize-1);
    if (rest != 0) scounts[gsize-1] = rest;

    char rbufs[strlen];
    
    // Distribute jobs
    MPI_Scatterv(str, scounts, displs, sendtype,  \
                 rbufs, stride, recvtype,           \
                 ROOT, MPI_COMM_WORLD);

    reverse_str(&rbufs[displs[rank]], scounts[rank]);
    
    MPI_Finalize();
    free(scounts);
    free(displs);
    return;
}
