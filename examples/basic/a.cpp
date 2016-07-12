
#include <stdio.h>
#include <mpi.h>

int main (int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 2) {
    MPI_Finalize();
    return 0;
  }

  int data;
  if (rank == 0) {
    data = 11;
    MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("[%d] Send data: %d\n", rank, data);
  } else if (rank == 1) {
    MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("[%d] Received data: %d\n", rank, data);
  }

  MPI_Finalize();
  return 0;
}
