#include <mpi.h>
#include <iostream>

bool fTest (int x, int y, int z) {
  return ((x * y) % (z / 2)) == 0;
}

int main (int argc, char **argv) {
  enum tags_t { T };

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int i = 0, j = 0;
  while (j <= 3) {
  // while (true) {

    int x = 3, y = 5;

    if (fTest(y, rank, size)) {
      std::cout << "rank: " << rank << std::endl;
      int next = (rank + 1) % size;
      // MPI_Request request;
      MPI_Send(&rank, 1, MPI_INT, next, T, MPI_COMM_WORLD);
    }

    int data;
    MPI_Request request;
    MPI_Irecv(&data, 1, MPI_INT, MPI_ANY_SOURCE, T, MPI_COMM_WORLD, &request);

    x = y + i;
    i++;

    MPI_Status status;
    MPI_Wait(&request, &status);
    y = data;
    std::cout << "loop: " << i << ", "
              << "rank: " << rank
              << std:: endl;
    // break;
    i = i % 2;
    if (i == 1) {
      j++;
    }
  }

  MPI_Finalize();
  return 0;
}
