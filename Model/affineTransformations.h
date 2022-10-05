#ifndef SRC_MODEL_AFFINETRANSFORMATIONS_H_
#define SRC_MODEL_AFFINETRANSFORMATIONS_H_

#include <math.h>

#include "s21_matrix_oop.h"

namespace s21 {
void translateX(S21Matrix *loadMatrix, const float x);
void translateY(S21Matrix *loadMatrix, const float y);
void translateZ(S21Matrix *loadMatrix, const float z);
void rotateX(S21Matrix *loadMatrix, const float radian);
void rotateY(S21Matrix *loadMatrix, const float radian);
void rotateZ(S21Matrix *loadMatrix, const float radian);
void scale(S21Matrix *loadMatrix, const float scale);
}  //  namespace s21

#endif  //  SRC_MODEL_AFFINETRANSFORMATIONS_H_
