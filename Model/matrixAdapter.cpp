#include "matrixAdapter.h"

namespace s21 {
QMatrix4x4 MatrixAdapter::returnQmatrix4x4(S21Matrix matrixS21) {
  QMatrix4x4 qMatrix;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      qMatrix(i, j) = matrixS21(i, j);
    }
  }

  return qMatrix;
}

S21Matrix MatrixAdapter::returnS21matrix(QMatrix4x4 matrixQm) {
  S21Matrix s21Matrix;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      s21Matrix(i, j) = matrixQm(i, j);
    }
  }

  return s21Matrix;
}
}  // namespace s21
