#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char** argv)
{
    double value;
    int size, rank;
    MPI_Status s;
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    value=MPI_Wtime();
    printf("MPI Process %d of %d (value=%f)\n", rank, size, value);
    
    if(rank==0)
        printf("Value : %f\n",value);

    if (rank>0)
        MPI_Recv(&value, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &s);
    if (rank<size-1)
        MPI_Send(&value, 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
    if (rank==size-1)
        printf("Value from MPI Process 0: %f\n",value);
    MPI_Finalize ();
}