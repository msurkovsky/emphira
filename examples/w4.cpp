
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
    for (int s = 1; s < size; s++) {
      MPI_Recv(&ns[s - 1], 1, MPI_INT, s, TAG_1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < size - 1; i++) {
      std::cout << ns[i] << ", ";
    }
    std::cout << std::endl;

    delete[] ns;
  } else {
    int n = rank;
    MPI_Send(&n, 1, MPI_INT, 0, TAG_1, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
