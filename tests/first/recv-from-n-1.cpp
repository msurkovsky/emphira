
#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[]) {

  enum tags_t { T };

  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    int v;
    for (int p = 1; p < size; p++) {
      MPI_Recv(&v, 1, MPI_INT, p, T, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // std::cout << "recvd from [" << p << "]: " << v << std::endl;
    }
  } else {
    int msg = 7;
    MPI_Send(&msg, 1, MPI_INT, 0, T, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
