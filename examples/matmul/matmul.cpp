
#include <mpi.h>
#include <cmath>
#include <iostream>
#include <sstream>

#include "matrix.h"

int next_a(int p, int n) {
  int r = p / n;
  int c  = p % n;
  return ((r + 1) % n) * n + c;
}

int next_b(int p, int n) {
  int r = p / n;
  int c = p % n;
  return r * n + ((c + 1) % n);
}

int main (int argc, char **argv) {
  enum tags_t { A, B, C };

  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = sqrt(size);

  Matrix *matrix_c;

  // distribution ==============================================================
  Matrix sub_a, sub_b, sub_c;
  char *data_buff[2];

  if (rank == 0) {
    Matrix matrix_a(4, 4, {
        1,1,2,2,
        1,1,2,2,
        3,3,4,4,
        3,3,4,4
    });

    Matrix matrix_b(4, 4, {
        4,4,2,2,
        4,4,2,2,
        3,3,1,1,
        3,3,1,1
    });

    matrix_c = new Matrix(matrix_a.rows(), matrix_b.cols());

    for (int p = 1; p < size; p++) {
      int i = p / n;
      int j = p % n;
      int k = ceil((2 * n - 1 - i - j) % n);

      // A(j, k)
      int sub_rows = matrix_a.rows() / n;
      int sub_cols = matrix_a.cols() / n;
      sub_a = matrix_a.extract_n(
        j * sub_rows,
        k * sub_rows,
        sub_rows,
        sub_cols
      );

      data_buff[0] = new char[sub_a.get_byte_size()];
      sub_a.serialize(data_buff[0]);
      // TODO: may have a problem with standard send (blocking one)
      MPI_Send(data_buff[0], sub_a.get_byte_size(), MPI_BYTE, p, A, MPI_COMM_WORLD);

      // B(k, i)
      sub_rows = matrix_b.rows() / n;
      sub_cols = matrix_b.cols() / n;
      sub_b = matrix_b.extract_n(
        k * sub_rows,
        i * sub_rows,
        sub_rows,
        sub_cols
      );

      data_buff[1] = new char[sub_b.get_byte_size()];
      sub_b.serialize(data_buff[1]);
      MPI_Send(data_buff[1], sub_b.get_byte_size(), MPI_BYTE, p, B, MPI_COMM_WORLD);
    }

    // extract a matrix for zero
    int p = 0;
    int i = p / n;
    int j = p % n;
    int k = ceil((2 * n - 1 - i - j) % n);

    int sub_rows = matrix_a.rows() / n;
    int sub_cols = matrix_a.cols() / n;
    // A(j, k)
    sub_a = matrix_a.extract_n(
      j * sub_rows,
      k * sub_rows,
      sub_rows,
      sub_cols
    );

    sub_rows = matrix_b.rows() / n;
    sub_cols = matrix_b.cols() / n;
    // B(k, i)
    sub_b = matrix_b.extract_n(
      k * sub_rows,
      i * sub_rows,
      sub_rows,
      sub_cols
    );

  } else {
    MPI_Status status;
    MPI_Probe(0, A, MPI_COMM_WORLD, &status);
    int msg_size;
    MPI_Get_count(&status, MPI_BYTE, &msg_size);

    data_buff[0] = new char[msg_size];
    MPI_Recv(data_buff[0], msg_size, MPI_BYTE, 0, A, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sub_a.deserialize(data_buff[0]);


    MPI_Probe(0, B, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_BYTE, &msg_size);

    data_buff[1] = new char[msg_size];
    MPI_Recv(data_buff[1], msg_size, MPI_BYTE, 0, B, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sub_b.deserialize(data_buff[1]);
  }
  sub_c = Matrix(sub_a.rows(), sub_b.cols());

  // computation ===============================================================
  sub_c += sub_a * sub_b;

  MPI_Request requests[2];
  MPI_Status statuses[2];
  for (int i = 1; i < n; i++) { // from 1 because the first round is done
                                // just after initial distribution

    // the communication is done in two waves
    // first each process send and receive A matrix
    // and in the other one B matrix is send and received
    sub_a.serialize(data_buff[0]);
    MPI_Isend(data_buff[0],
              sub_a.get_byte_size(),
              MPI_BYTE,
              next_a(rank, n),
              A,
              MPI_COMM_WORLD,
              &requests[0]);

    MPI_Irecv(data_buff[1], // use the second buffer for receive in order
                            // not to rewrite the first one
              sub_a.get_byte_size(),
              MPI_BYTE,
              MPI_ANY_SOURCE, // TODO: I know the originator process
              A,
              MPI_COMM_WORLD,
              &requests[1]);

    MPI_Waitall(2, requests, statuses);
    sub_a.deserialize(data_buff[1]);

    sub_b.serialize(data_buff[0]);
    MPI_Isend(data_buff[0],
              sub_b.get_byte_size(),
              MPI_BYTE,
              next_b(rank, n),
              B,
              MPI_COMM_WORLD,
              &requests[0]);

    MPI_Irecv(data_buff[1],
              sub_b.get_byte_size(),
              MPI_BYTE,
              MPI_ANY_SOURCE, // TODO:
              B,
              MPI_COMM_WORLD,
              &requests[1]);

    MPI_Waitall(2, requests, statuses);
    sub_b.deserialize(data_buff[1]);

    sub_c += sub_a * sub_b;
  }

  // gather partial results ====================================================
  if (rank == 0) {
    matrix_c->insert(sub_c, 0, 0);

    int rs = matrix_c->rows() / n;
    int cs = matrix_c->cols() / n;

    MPI_Status status;
    int i, j, p;
    for (int k = 1; k < size; k++) {
      MPI_Recv(data_buff[0], sub_c.get_byte_size(),
               MPI_BYTE,
               MPI_ANY_SOURCE,
               C,
               MPI_COMM_WORLD,
               &status);

      sub_c.deserialize(data_buff[0]);

      p = status.MPI_SOURCE;
      i = p / n;
      j = p % n;
      matrix_c->insert(sub_c, j * rs, i * cs);
    }

    std::cout << *matrix_c;

    delete matrix_c; // delete only on zero process, only there is initialized
  } else {
    sub_c.serialize(data_buff[0]);
    MPI_Send(data_buff[0],
             sub_c.get_byte_size(),
             MPI_BYTE,
             0,
             C,
             MPI_COMM_WORLD);
  }

  delete[] data_buff[0];
  delete[] data_buff[1];

  MPI_Finalize();
  return 0;
}
