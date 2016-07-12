
#include <stdio.h>
#include <mpi.h>

int main (int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int data;
  if (rank == 0) {
    data = 11;
    for (int i = 0; i < size; i++) { // what if i = 0?
      data += i;
      MPI_Send(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      printf("[%d](%d) Send data: %d\n", rank, i, data);
    }
  }

  MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  printf("[%d] Received data: %d\n", rank, data);

  MPI_Finalize();
  return 0;
}
