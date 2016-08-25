
#include <mpi.h>
#include <iostream>

int main (int argc, char **argv) {
  enum tags_t { T };

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    int matched;
    MPI_Status status;
    int start_loop = 1;
    int data;
    MPI_Iprobe(1, T, MPI_COMM_WORLD, &matched, &status);
    if (matched) {
      MPI_Recv(&data,
               1, MPI_INT,
               status.MPI_SOURCE, status.MPI_TAG,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      start_loop = 2;
      std::cout << "Received probed message, "
                << "[" << rank << "]: "
                << data
                << std::endl;
    }

    for (int i = start_loop; i < size; i++) {
      MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, T, MPI_COMM_WORLD, &status);
      std::cout << "Received message, "
                << "[" << rank << "]: "
                << data
                << std::endl;
    }
  } else {
    MPI_Send(&rank, 1, MPI_INT, 0, T, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
