
#include <stdio.h>
#include <mpi.h>

int main (int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int data = 11;
  int i = 0, n = 10;
  while (i < n) {
    if (rank == 0) {
      data += i;
      // MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); // for #procs=1 invalid rank
      MPI_Send(&data, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD); // for #procs=1 deadlock
      printf("[%d](%d) Send data: %d\n", rank, i, data);
      MPI_Recv(&data, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("[%d](%d) Received data: %d\n", rank, i, data);
    } else {
      MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("[%d](%d) Received data: %d\n", rank, i, data);
      data += 3;
      MPI_Send(&data, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
      printf("[%d](%d) Send data: %d\n", rank, i, data);
    }
    i += 1;
  }

  MPI_Finalize();
  return 0;
}
