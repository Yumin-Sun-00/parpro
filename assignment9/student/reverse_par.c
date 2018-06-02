#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 
#include "helper.h"
#include <math.h>

void reverse(char *str, int strlen)
{
    int size, rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    int sublen = ceil(strlen / (size-1));
 
    char* starts[size-1];
    for(int i = 0 ; i < size-1; i++ ){starts[i] = str + sublen * i;}

    static int FROM_ID = 0;

    // Distribute jobs using the first process
    if( rank == 0 )
    {
        for(int i = 0; i < size-1; i++)
        {
            MPI_Send(&starts[i], sublen, MPI_CHAR, i+1, 0, MPI_COMM_WORLD);
        }
    }
    // Recieve jobs from process 0 and reverse the sub-string
    else
    {
        MPI_Recv(&starts[rank-1], sublen, MPI_CHAR, FROM_ID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        reverse_str(starts[rank-1], sublen);
    }
    MPI_Finalize();
    return;
}
