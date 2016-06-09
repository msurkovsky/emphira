
#include <mpi.h>
#include <stdio.h>

int main (int argc, char **argv) {

  int rank;

  int x = 0;
  int z = 2;
  int b = 7;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    x = x + 1;
    b = x * 3;
    // TODO: only for two process for now
    MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else {
    int y;
    // NOTE: only one process match the data from process zero
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
