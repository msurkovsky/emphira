
#include <stdio.h>
#include <mpi.h>

int main (int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = 10;
  int data;
  if (rank == 0) {
    data = 11;
    for (int i = 0; i < n; i++) { // communication is independent on 'i'
      data += i;
      MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      printf("[%d](%d) Send data: %d\n", rank, i, data);
    }
  } else if (rank == 1) {
    for (int i = 0; i < 10; i++) {
      MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("[%d](%d) Received data: %d\n", rank, i, data);
    }
  }

  MPI_Finalize();
  return 0;
}
