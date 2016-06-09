
#include <mpi.h>
#include <stdio.h>

int main (int argc, char **argv) {

  int rank, size;

  int x = 0;
  int z = 2;
  int b = 7;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      x = x + i;
      b = x * 3;
      MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  } else {
    int y;
    MPI_Recv(&y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    z = b * y;
  }

  int sum;
  MPI_Reduce(&z, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  printf("z[%d]: %d\n", rank, z);
  if (rank == 0) {
    printf("Sum: %d\n", sum);
  }

  MPI_Finalize();
  return 0;
}
