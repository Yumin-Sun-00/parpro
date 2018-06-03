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
    int stride = ceil( (float) strlen / (float) gsize);

    //printf("%d: ", stride);

    // MPI_Scatterv params
    int *displs, *scounts; 
    displs = (int *)malloc(gsize*sizeof(int)); 
    scounts = (int *)malloc(gsize*sizeof(int));
    for (int i=0; i < gsize; ++i) { 
        displs[i] = i*stride; 
        scounts[i] = stride; 
    } 
    int rest = strlen - stride * (gsize-1);
    if (rest != 0) scounts[gsize-1] = rest;

    char rbufs[gsize][stride];

    // Distribute jobs
    MPI_Scatterv(str, scounts, displs, MPI_CHAR,  \
                     rbufs[rank], stride, MPI_CHAR,       \
                     0, MPI_COMM_WORLD);

    /* print what each process received*/
    printf("Rank %d: ", rank);
    print(rbufs[rank],scounts[rank]);
    printf("\n");

    reverse_str(rbufs[rank], scounts[rank]);
    printf("Reversed:%s",rbufs[rank]);
    printf("\n");
    //int revdisp = 
    /* Gather
    MPI_Gatherv(
    sendbuf, sendcount, sendtype,
    recvbuf, recvcounts, displs, recvtype,
    root, comm); */
    MPI_Gatherv(rbufs, stride, MPI_CHAR, str, scounts, displs, MPI_CHAR,0, MPI_COMM_WORLD);
    
    return;
}
