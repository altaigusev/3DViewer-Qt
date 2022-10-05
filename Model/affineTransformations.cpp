#include "affineTransformations.h"

namespace s21 {
void translateX(S21Matrix *loadMatrix, const float x) { loadMatrix->operator()(0, 3) = x; }

void translateY(S21Matrix *loadMatrix, const float y) { loadMatrix->operator()(1, 3) = y; }

void translateZ(S21Matrix *loadMatrix, const float z) { loadMatrix->operator()(2, 3) = z; }

void rotateX(S21Matrix *loadMatrix, const float radian) {
  float deg = radian * float(M_PI / 180);
  float c = cos(deg);
  float s = sin(deg);
  float tmp;
  loadMatrix->operator()(1, 0) = (tmp = loadMatrix->operator()(1, 0)) * c + loadMatrix->operator()(2, 0) * s;
  loadMatrix->operator()(2, 0) = loadMatrix->operator()(2, 0) * c - tmp * s;
  loadMatrix->operator()(1, 1) = (tmp = loadMatrix->operator()(1, 1)) * c + loadMatrix->operator()(2, 1) * s;
  loadMatrix->operator()(2, 1) = loadMatrix->operator()(2, 1) * c - tmp * s;
  loadMatrix->operator()(1, 2) = (tmp = loadMatrix->operator()(1, 2)) * c + loadMatrix->operator()(2, 2) * s;
  loadMatrix->operator()(2, 2) = loadMatrix->operator()(2, 2) * c - tmp * s;
  loadMatrix->operator()(1, 3) = (tmp = loadMatrix->operator()(1, 3)) * c + loadMatrix->operator()(2, 3) * s;
  loadMatrix->operator()(2, 3) = loadMatrix->operator()(2, 3) * c - tmp * s;
}

void rotateY(S21Matrix *loadMatrix, const float radian) {
  float deg = radian * float(M_PI / 180);
  float c = cos(deg);
  float s = sin(deg);
  float tmp;
  loadMatrix->operator()(2, 0) = (tmp = loadMatrix->operator()(2, 0)) * c + loadMatrix->operator()(0, 0) * s;
  loadMatrix->operator()(0, 0) = loadMatrix->operator()(0, 0) * c - tmp * s;
  loadMatrix->operator()(2, 1) = (tmp = loadMatrix->operator()(2, 1)) * c + loadMatrix->operator()(0, 1) * s;
  loadMatrix->operator()(0, 1) = loadMatrix->operator()(0, 1) * c - tmp * s;
  loadMatrix->operator()(2, 2) = (tmp = loadMatrix->operator()(2, 2)) * c + loadMatrix->operator()(0, 2) * s;
  loadMatrix->operator()(0, 2) = loadMatrix->operator()(0, 2) * c - tmp * s;
  loadMatrix->operator()(2, 3) = (tmp = loadMatrix->operator()(2, 3)) * c + loadMatrix->operator()(0, 3) * s;
  loadMatrix->operator()(0, 3) = loadMatrix->operator()(0, 3) * c - tmp * s;
}

void rotateZ(S21Matrix *loadMatrix, const float radian) {
  float deg = radian * float(M_PI / 180);
  float c = cos(deg);
  float s = sin(deg);
  float tmp;
  loadMatrix->operator()(0, 0) = (tmp = loadMatrix->operator()(0, 0)) * c + loadMatrix->operator()(1, 0) * s;
  loadMatrix->operator()(1, 0) = loadMatrix->operator()(1, 0) * c - tmp * s;
  loadMatrix->operator()(0, 1) = (tmp = loadMatrix->operator()(0, 1)) * c + loadMatrix->operator()(1, 1) * s;
  loadMatrix->operator()(1, 1) = loadMatrix->operator()(1, 1) * c - tmp * s;
  loadMatrix->operator()(0, 2) = (tmp = loadMatrix->operator()(0, 2)) * c + loadMatrix->operator()(1, 2) * s;
  loadMatrix->operator()(1, 2) = loadMatrix->operator()(1, 2) * c - tmp * s;
  loadMatrix->operator()(0, 3) = (tmp = loadMatrix->operator()(0, 3)) * c + loadMatrix->operator()(1, 3) * s;
  loadMatrix->operator()(1, 3) = loadMatrix->operator()(1, 3) * c - tmp * s;
}

void scale(S21Matrix *loadMatrix, const float scale) {
  loadMatrix->operator()(0, 0) *= scale;
  loadMatrix->operator()(0, 1) *= scale;
  loadMatrix->operator()(0, 2) *= scale;
  loadMatrix->operator()(1, 0) *= scale;
  loadMatrix->operator()(1, 1) *= scale;
  loadMatrix->operator()(1, 2) *= scale;
  loadMatrix->operator()(2, 0) *= scale;
  loadMatrix->operator()(2, 1) *= scale;
  loadMatrix->operator()(2, 2) *= scale;
}
}  //  namespace s21
