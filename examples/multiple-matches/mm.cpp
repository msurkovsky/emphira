#include <mpi.h>
#include <iostream>

int main (int argc, char **argv) {
  enum tags_t { T };

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    int i = 0;
    while (i < 20) { // TODO: how to recognize whether or not the condition is right?
                     // Do I want to do something like that?
      int x = 3, y;
      MPI_Send(&x, 1, MPI_INT, 1, T, MPI_COMM_WORLD);
      MPI_Recv(&y, 1, MPI_INT, 1, T, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      std::cout << "[" << rank << "]: "
                << "s: " << x << ", "
                << "r: " << y << ", "
                << "i: " << i
                << std::endl;
      i += 1;
    }
  } else if (rank == 1) {
    int i = 0;
    while (i < 10) { // dependent condition
      int y = 7, x;
      // if (i % 3 == 0) {
        MPI_Recv(&x, 1, MPI_INT, 0, T, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // }
      MPI_Send(&y, 1, MPI_INT, 0, T, MPI_COMM_WORLD);
      MPI_Recv(&x, 1, MPI_INT, 0, T, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&y, 1, MPI_INT, 0, T, MPI_COMM_WORLD);
      std::cout << "[" << rank << "]: "
                << "s: " << y << ", "
                << "r: " << x << ", "
                << "i: " << i
                << std::endl;
      i += 1;
    }
  }
  MPI_Finalize();
  return 0;
}
