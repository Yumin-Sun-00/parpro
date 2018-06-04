#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 
#include "helper.h"
#include <math.h>

void reverse(char *str, int strlen)
{
    printf("strlen: %d", strlen);    
    int gsize, rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &gsize);
    int stride = ceil( (float) strlen / (float) gsize);
    printf("stride %d\n",stride);
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
    
    int *rev_displs;
    rev_displs = (int*)malloc(gsize*sizeof(int));
    for(int i =0; i < gsize; i++)
    {
        rev_displs[i]=strlen-i*stride-scounts[i] ;
    }

    char rbufs[scounts[rank]];

    // Distribute jobs
    MPI_Scatterv(str, scounts, displs, MPI_CHAR,  \
                      rbufs, scounts[rank], MPI_CHAR,       \
                     0, MPI_COMM_WORLD);

    /* print what each process received*/
    printf("Chunck from process %d is ",rank);
    print(rbufs, scounts[rank]);
    printf("rank %d:, displ: %d, scounts: %d, rev_displs: %d", rank, displs[rank],scounts[rank],rev_displs[rank]);
    printf("\n");
    reverse_str(rbufs, scounts[rank]);
    printf("Reversed:");
    printf("\n");
    print(rbufs,scounts[rank]);
    strncpy(&str[rev_displs[rank]], rbufs, scounts[rank]);    
    
   free(displs);
free(scounts);
free(rev_displs);

    return;
}
