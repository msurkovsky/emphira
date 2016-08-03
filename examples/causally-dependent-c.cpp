#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    int *data = new int[size - 1];
    int result = 0;
    for (int i = 0; i < size - 1; i++) {
      MPI_Recv(&data[i], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      result += (i % 2 == 0 ? 1 : - 1) *data[i];
    }
    printf("%d\n", result);
    delete[] data;
  } else {
    MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
