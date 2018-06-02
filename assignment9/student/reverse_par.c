#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h> 
#include "helper.h"

  
void reverse(char *str, int strlen)
{
        // parallelize this function and make sure to call reverse_str()
        // on each processor to reverse the substring.

    int np, rank;
    MPI_Init(str,&strlen);
    MPI_Comm_size(MPI_COMM_WORLD, &np); // Number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Process id
    
    char* substr = str;
    int substrlen = int(strlen / np);

    if (rank == 0)
    {
        reverse_str(subtr, substrlen);
    }
    MPI_Finalize ();

}
