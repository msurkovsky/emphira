
#include <mpi.h>
#include <iostream>

int main (int argc, char **argv) {
  enum tags_t { TAG_1 };

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    int *ns = new int[size - 1];
    MPI_Request *requests = new MPI_Request[size - 1];
    MPI_Status *statuses = new MPI_Status[size - 1];

    for (int i = 0; i < size - 1; i++) {
      MPI_Irecv(&ns[i],
                1,
                MPI_INT,
                MPI_ANY_SOURCE,
                TAG_1,
                MPI_COMM_WORLD,
                &requests[i]);
    }
    MPI_Waitall(size - 1, requests, statuses);

    for (int i = 0; i < size - 1; i++) {
      std::cout << ns[i] << ", ";
    }
    std::cout << std::endl;

    delete[] ns;
    delete[] requests;
    delete[] statuses;
  } else {
    int n = rank;
    MPI_Send(&n, 1, MPI_INT, 0, TAG_1, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
