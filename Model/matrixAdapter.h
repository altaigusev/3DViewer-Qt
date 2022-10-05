#ifndef SRC_MODEL_MATRIXADAPTER_H_
#define SRC_MODEL_MATRIXADAPTER_H_

#include "QMatrix4x4"
#include "s21_matrix_oop.h"

namespace s21 {
class MatrixAdapter : public QMatrix4x4 {
 private:
  MatrixAdapter() {}
  ~MatrixAdapter() {}
  MatrixAdapter(MatrixAdapter const&) = delete;
  MatrixAdapter& operator=(MatrixAdapter const&) = delete;

 public:
  static MatrixAdapter& Instance() {
    // согласно стандарту, этот код ленивый и потокобезопасный
    static MatrixAdapter s;
    return s;
  }
  QMatrix4x4 returnQmatrix4x4(S21Matrix matrixS21);
  S21Matrix returnS21matrix(QMatrix4x4 matrixQm);
};
}  // namespace s21
#endif  //  SRC_MODEL_MATRIXADAPTER_H_
