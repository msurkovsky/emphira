
#include <mpi.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>
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

  Matrix<T>* take (int start_x, int start_y, int width, int height) {
    assert(start_x < this->width && start_y < this->height);

    int w = std::min(this->width, width);
    int h = std::min(this->height, height);

    Matrix<T> *m = new Matrix<T>(w, h);
    for (int i = 0, x = start_x; x < w; i++, x++) {
      for (int j = 0, y = start_y; y < h; j++, y++) {
        m->data[i][j] = data[x][y];
      }
    }

    return m;
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
  }

  size_t byte_size () const {
    return sizeof(T) * width * height + 2 * sizeof (int);
  }

  void print () const {
    int i;
    for (int j = 0; j < height; j++) {
      for (i = 0; i < width - 1; i++) {
        std::cout << data[i][j] << " ";
      }
      std::cout << data[i][j] << std::endl;
    }
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
  friend std::ostream& operator<< (std::ostream& stream, const Matrix<T>& m);

  template<typename A>
  friend std::istream& operator>> (std::istream& stream, Matrix<A>& m);

  template<typename A>
  friend Matrix<A> operator+ (const Matrix<A>& m1, const Matrix<A>& m2);

  template<typename A>
  friend Matrix<A> operator* (const Matrix<A>& m1, const Matrix<A>& m2);
};

template<typename T>
std::ostream& operator<< (std::ostream& stream, const Matrix<T>& m) {

  stream << m.width << " " << m.height << std::endl;
  for (int i = 0; i < m.width; i++) {
    int j = 0;
    for (; j < m.height - 1; j++) {
      stream << m.data[i][j] << " ";
    }
    stream << m.data[i][j] << std::endl;
  }
  return stream;
}

template<typename T>
std::istream& operator>> (std::istream& stream, Matrix<T>& m) {
  stream >> m.width >> m.height;

  if (m.data != NULL) {
    for (int i = 0; i < m.width; i++) {
      delete[] m.data[i];
    }
    delete[] m.data;
  }

  m.data = new T*[m.width];
  for (int i = 0; i < m.width; i++) {
    m.data[i] = new T[m.height];
    for (int j = 0; j < m.height; j++) {
      stream >> m.data[i][j];
    }
  }

  return stream;
}

template<typename T>
Matrix<T> operator+ (const Matrix<T>& m1, const Matrix<T>& m2) {
  assert(m1.width == m2.width && m1.height == m2.height);

  int w = m1.width;
  int h = m1.height;
  Matrix<T> m(w, h);
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      m.data[i][j] = m1.data[i][j] + m2.data[i][j];
    }
  }

  return m;
}

template<typename T>
Matrix<T> operator* (const Matrix<T>& m1, const Matrix<T>& m2) {
  assert(m1.width == m2.height);

  Matrix<T> m(m2.width, m1.height);
  for (int i = 0; i < m2.width; i++) {
    for (int j = 0; j < m1.height; j++) {
      for (int k = 0; k < m2.height; k++) { // == m1.width
        m.data[i][j] += m1.data[k][j] * m2.data[i][k];
      }
    }
  }

  return m;
}

int main (int argc, char **argv) {


  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    Matrix<int> m(2, 3);
    m.fill(1);
    m.data[0][0] = 3;
    std::cout << "size: " << m.byte_size() << std::endl;
    std::cout << "sizeof int" << sizeof(int) << std::endl;
    std::stringstream ss;
    ss << m;
    std::cout << m << std::endl;
    const std::string& tmp = ss.str();
    const char *data = tmp.c_str();
    MPI_Send(data, m.byte_size(), MPI_BYTE, 1, 0, MPI_COMM_WORLD);
  }

  if (rank == 1) {
    MPI_Status status;
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    int size;
    MPI_Get_count(&status, MPI_BYTE, &size);

    char *buff = new char[size];
    MPI_Recv(buff, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);
    std::string s(buff);
    std::stringstream ss(s);

    Matrix<int> m;
    ss >> m;
    m.print();

    delete[] buff;

    Matrix<int> m2(4, 2);
    m2.fill(2);
    m2.print();


    // std::cout << m << std::endl << m2 << std::endl;
    Matrix<int> dm = m * m2;
    dm.print();
  }

  MPI_Finalize();
  return 0;
}
