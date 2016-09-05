
#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>

template<typename T>
struct Matrix {
  int width;
  int height;

  T **data = NULL;

  Matrix() : width(0), height(0) { }

  Matrix(int width, int height)
      : width(width), height(height) {

    data = new T*[width];
    for (int i = 0; i < width; i++) {
      data[i] = new T[height];
    }
  }

  Matrix (const Matrix<T> &m) {
    width = m.width;
    height = m.height;

    data = new T*[width];
    for (int i = 0; i < width; i++) {
      data[i] = new T[height];
      memcpy(data[i], m.data[i], height * sizeof(T));
    }
  }

  ~Matrix() {
    for (int i = 0; i < width; i++) {
      delete[] data[i];
    }
    delete[] data;
  }

  void fill (T value) {
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        data[i][j] = value;
      }
    }
  }

  void put (const Matrix<T> injected, int start_x, int start_y) {
    assert(start_x < width && start_y < height);

    int w = std::min(injected.width, width);
    int h = std::min(injected.height, height);
    for (int i = 0, x = start_x; x < w; i++, x++) {
      for (int j = 0, y = start_y; y < h; j++, y++) {
        data[x][y] = injected[i][j];
      }
    }
  };

  Matrix<T>& take (int start_x, int width, int start_y, int height) {
    assert(start_x < this->width && start_y < this->height);

    int w = std::min(this->width, width);
    int h = std::min(this->height, height);

    Matrix<T> *m = new Matrix<T>(w, h);
    for (int i = 0, x = start_x; x < w; i++, x++) {
      for (int j = 0, y = start_y; y < h; j++, y++) {
        m->data[i][j] = data[x][y];
      }
    }

    return *m;
  }

  Matrix<T> &operator= (const Matrix<T> &other) {
    if (this != &other) {
      if (width != other.width || height != other.height) {
        if (data != NULL) {
          // delete old one
          for (int i = 0; i < width; i++) {
            delete[] data[i];
          }
          delete[] data;
        }

        // re-allocate
        width = other.width;
        height = other.height;

        data = new T*[width];
        for (int i = 0; i < width; i++) {
          data[i] = new T[height];
        }
      }

      // copy
      for (int i = 0; i < width; i++) {
        memcpy(data[i], other.data[i], height * sizeof(T));
      }
    }

    return *this;
  }

  template<typename A>
  friend std::ostream& operator<< (std::ostream& stream, const Matrix<A>& m);
};

template<typename T>
std::ostream& operator<< (std::ostream& stream, const Matrix<T>& m) {
  stream << "width: " << m.width << ", "
         << "height: " << m.height << std::endl;

  for (int i = 0; i < m.width; i++) {
    for (int j = 0; j < m.height; j++) {
      stream << m.data[i][j] << " ";
    }
    stream << std::endl;
  }
  return stream;
}

int main (int argc, char **argv) {

  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Matrix<int> m(10, 10);
  m.fill(1);

  Matrix<int> m2(m);
  m2.data[4][4] = 3;

  std::cout << "m: " << m << std::endl;
  std::cout << "m2: " << m2 << std::endl;

  return 0;
}
