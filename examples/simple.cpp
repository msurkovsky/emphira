
#include "mpi.h"
// #include <iostream>

int main(int argc, char **argv) {

  int rank, size;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int data = 10;

  if (rank == 0) {
    data += 1;
    MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else {
    MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // std::cout << "Process " << rank << " received the number: " << data << std::endl;
  }

  MPI_Finalize();
  return 0;
}
