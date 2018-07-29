#include <mpi.h>
#include <stdio.h>

// mpicc src/exercise2.c -o bin/exercise2
// mpirun -np 16 ./bin/exercise2

int main(int argc, char* argv[]) {
	int         rank, size, nameLength, status, left, right, up, down, idx;
	char        name[MPI_MAX_PROCESSOR_NAME];
	MPI_Request request[2];

	MPI_Init (&argc, &argv);
	
	nameLength = MPI_MAX_PROCESSOR_NAME;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(name, &nameLength);

	printf("This is rank %.2d of %.2d running on node %s.\n", rank, size, name);

	status  = (rank == 0) ? 1 : 0;

	left  = (rank & 0xfffc) + ((rank + 3) % 4);
	right = (rank & 0xfffc) + ((rank + 1) % 4);
	up    = (rank + 12) % 16;
	down  = (rank +  4) % 16;

	if(rank == 0) {
		MPI_Send(&status, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
		MPI_Send(&status, 1, MPI_INT, down,  0, MPI_COMM_WORLD);
		MPI_Recv(&status, 1, MPI_INT, left,  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&status, 1, MPI_INT, up,    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	} else {
		MPI_Irecv(&status, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &request[0]);
		MPI_Irecv(&status, 1, MPI_INT, up,   0, MPI_COMM_WORLD, &request[1]);

		MPI_Waitany(2, request, &idx, MPI_STATUS_IGNORE);

		MPI_Send(&status, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
		MPI_Send(&status, 1, MPI_INT, down,  0, MPI_COMM_WORLD);
	}

	printf("Rank %.2d: %d\n", rank, status);

	MPI_Finalize();

	return 0;
}
