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
    static int FROM_ID = 0;

    int sublen = ceil(strlen / (size-1));
    static int restlen= sublen * (size-1) - strlen;

    // Distribute jobs using the first process
    if ( rank == 0 )
    {
        for(int i = 0; i < size-1; i++)
        {
            MPI_Send(str + sublen * i, sublen, MPI_CHAR, i+1, 0, MPI_COMM_WORLD);
        }
    }
    // Recieve jobs from process 0 and reverse the sub-string
    else if ( rank == size-1 && restlen != 0 )
    {
        MPI_Recv(str + sublen * (rank-1), restlen, MPI_CHAR, FROM_ID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        reverse_str(str + sublen * (rank-1), sublen);
    }
    else
    {
        MPI_Recv(str + sublen * (rank-1), sublen, MPI_CHAR, FROM_ID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        reverse_str(str + sublen * (rank-1), sublen);
    }
    MPI_Finalize();
    return;
}
