#include<stdio.h>
#include<string.h>
#include<mpi.h>

int main(int argc, char *argv[]) {
	int my_rank;
	int np;
	int source;
	int tag = 0;
	char message[100];
	MPI_Status status;

	/* Setup MPI */
	MPI_Init(&argc, &argv);
	/* Get process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	/* Get number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	if (my_rank != 0) {
		sprintf(message, "Greetings from process %d", my_rank);
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
	}
	else {
		for (source = 1; source < np; source++) {
			MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
			printf("%s\n", message);
		}
	}

	MPI_Finalize();
	return 0;
}
