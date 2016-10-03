#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <cstring>
#include <cassert>

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>

class Matrix {

  public:
  Matrix() : _rows(0), _columns(0) { }

  Matrix(size_t rows, size_t columns, double init_value=0)
      : _rows(rows), _columns(columns) {

    data.resize(_rows * _columns, init_value);
  }

  Matrix(size_t rows, size_t columns, double* values)
      : _rows(rows), _columns(columns) {

    data.assign(values, values + rows * columns);
  }

  Matrix(size_t rows, size_t columns, std::initializer_list<double> values)
      : _rows(rows), _columns(columns) {

    data.assign(values.begin(), values.end());
  }

  friend std::ostream& operator<<(std::ostream& os, const Matrix &m);

  Matrix operator*(const Matrix &other) {
    Matrix result(_rows, _columns);
    for (size_t i = 0; i < _rows; i++) {
      for (size_t j = 0; j < other.cols(); j++) {
        for (size_t k = 0; k < _columns; k++) {
          result[i][j] += (*this)[i][k] * other[k][j];
        }
      }
    }
    return result;
  }

  Matrix& operator+=(const Matrix &other) {
    for (size_t i = 0; i < _rows; i++) {
      for (size_t j = 0; j < _columns; j++) {
        (*this)[i][j] += other[i][j];
      }
    }
    return *this;
  }

  const double* operator[] (int row) const {
    // accessing elements from const value
    assert(row < _rows);
    return &data[row * _columns];
  }

  double* operator[](int row) {
    assert(row < _rows);
    return &data[row * _columns];
  };

  // get submatrix
  Matrix extract_n(int r, int c, int rc, int cc) const {
    Matrix sub(rc, cc);
    for (size_t i = 0; i < rc; i++) {
      for (size_t j = 0; j < cc; j++) {
        sub[i][j] = (*this)[i+r][j+c];
      }
    }
    return sub;
  }

  // set submatrix
  void insert(const Matrix &other, int r, int c) {
    for (size_t i = 0; i < other.rows(); i++) {
      for (size_t j = 0; j < other.cols(); j++) {
        (*this)[i+r][j+c] = other[i][j];
      }
    }
  }

  size_t rows() const {
    return _rows;
  }

  size_t cols() const {
    return _columns;
  }

  size_t get_byte_size() const {
    return 2 * sizeof(size_t) + _rows * _columns * sizeof(double);
  }

  void serialize (char *data) const {
    char *data_p = data;

    std::copy(&_rows, &_rows + 1, (size_t *) data_p);
    data_p += sizeof(size_t);

    std::copy(&_columns, &_columns + 1, (size_t *) data_p);
    data_p += sizeof(size_t);

    std::copy(this->data.begin(), this->data.end(), (double *) data_p);

    // NOTE: the old one way
    /* memcpy(data_p, &_rows, sizeof(size_t)); */
    /* data_p += sizeof(size_t); */

    /* memcpy(data_p, &_columns, sizeof(size_t)); */
    /* data_p += sizeof(size_t); */

    /* memcpy(data_p, &this->data[0], _rows * _columns * sizeof(double)); */
  };

  void deserialize(const char *data) {
    size_t *sizes_p = (size_t *) data;

    std::copy(sizes_p, sizes_p + 1, &_rows);
    sizes_p += 1;

    std::copy(sizes_p, sizes_p + 1, &_columns);
    sizes_p += 1;

    size_t size = _rows * _columns;
    double *data_p = (double *) sizes_p;
    this->data = std::vector<double>(data_p, data_p + size);

    // NOTE: the old one way
    /* memcpy(&_rows, data_p, sizeof(size_t)); */
    /* data_p += sizeof(size_t); */

    /* memcpy(&_columns, data_p, sizeof(size_t)); */
    /* data_p += sizeof(size_t); */

    /* this->data.resize(_rows * _columns); */
    /* memcpy(&this->data[0], data_p, _rows * _columns * sizeof(double)); */
  }

  protected:
    std::vector<double> data;
    size_t _rows;
    size_t _columns;
};

std::ostream& operator<<(std::ostream& os, const Matrix &m) {
  for(int i = 0; i < m.rows(); i++) {
    for(int j = 0; j < m.cols(); j++) {
      os << m[i][j] << " ";
    }
    os << std::endl;
  }
  os << std::endl;
  return os;
}

#endif // MATRIX_H
