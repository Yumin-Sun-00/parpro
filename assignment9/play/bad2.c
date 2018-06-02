#include <mpi.h>


int main (int argc, char* argv[])
{
    int rank, size, tmp;

    if(rank == 0)
    { 
        MPI_Recv(&tmp, 1, MPI_INT , mod(rank -1,size), 0, MPI_COMM_WORLD , MPI_STATUS_IGNORE);
        MPI_Send(&rank, 1, MPI_INT , mod(rank+1,size), 0,MPI_COMM_WORLD); 
    }
    else
    { 
        MPI_Send(&rank, 1, MPI_INT , mod(rank+1,size), 0,MPI_COMM_WORLD);
        MPI_Recv(&tmp, 1, MPI_INT , mod(rank -1,size), 0,MPI_COMM_WORLD , MPI_STATUS_IGNORE); 
    }
    MPI_Finalize();
    return 0;
}