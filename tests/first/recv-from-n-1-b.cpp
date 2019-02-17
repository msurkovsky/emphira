
#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[]) {

  enum tags_t { T };

  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    int *ns = new int[size - 1];
    MPI_Request *requests = new MPI_Request[size - 1];
    MPI_Status *statuses = new MPI_Status[size - 1];
    for (int p = 1; p < size; p++) {
      MPI_Irecv(&ns[p-1], 1, MPI_INT, p, T, MPI_COMM_WORLD, &requests[p-1]);
    }
    MPI_Waitall(size - 1, requests, statuses);

    // for (int p = 1; p < size ; p++) {
    //   std::cout << "recvd from [" << p << "]: " << ns[p-1] << std::endl;
    // }
  } else {
    int msg = rank;
    MPI_Send(&msg, 1, MPI_INT, 0, T, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
