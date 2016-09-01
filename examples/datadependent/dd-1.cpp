#include <mpi.h>
#include <iostream>

int main (int argc, char **argv) {
  enum tags_t { T };

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // int x = rank, y = (rank + 1) % size;
  int t = 0;
  int i = 0, j = 0;
  while (j <= 3) {

    if (t != rank) {
      int next = (rank + 1) % size;
      MPI_Send(&rank, 1, MPI_INT, next, T, MPI_COMM_WORLD);
    }

    int data;
    MPI_Request request;
    MPI_Irecv(&data, 1, MPI_INT, MPI_ANY_SOURCE, T, MPI_COMM_WORLD, &request);

    i++;

    MPI_Status status;
    MPI_Wait(&request, &status);
    t = data;
    std::cout << "[" << rank << "]: "
              << t << std::endl;

    i %= 2;
    if (i == 1) {
      j++;
    }
  }

  if (rank == 0) { // solve the problem of unmatched receive.
                   // TODO: compute automatically the ranks of processes
                   // that one will send a message that the other one unblock.
    MPI_Send(&rank, 1, MPI_INT, 1, T, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
