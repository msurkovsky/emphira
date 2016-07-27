#include <stdio.h>
#include <mpi.h>

int main (int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 3) {
    printf("Invalid number of processes. Three processes are expected.\n");
    return 0;
  }

  if (rank == 0) {
    int a, b;
    MPI_Recv(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&b, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int result = a - b;
    // printf("Result a - b = %d\n", result);
    printf("%d\n", result);
  } else if (rank == 1) {
    int a = 9;
    MPI_Send(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  } else if (rank == 2) {
    int b = 7;
    MPI_Send(&b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
