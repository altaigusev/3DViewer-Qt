#ifndef SRC_CONTROLLER_CONTROLLER_H_
#define SRC_CONTROLLER_CONTROLLER_H_

#include <QMatrix4x4>
#include <QString>

#include "../Model/loaderobj.h"
#include "../Model/matrixAdapter.h"

namespace s21 {
class SimpleObject3D;
class Controller {
 public:
  static Controller &Instance() {
    static Controller s;
    return s;
  }
  SimpleObject3D *loadObjectModel(const QString &path);
  void loadTexture(const QString &pathTexture);

  QMatrix4x4 transX(QMatrix4x4 modelMatrix, float xM);
  QMatrix4x4 transY(QMatrix4x4 modelMatrix, float yM);
  QMatrix4x4 transZ(QMatrix4x4 modelMatrix, float zM);

  QMatrix4x4 rtX(QMatrix4x4 modelMatrix, float rX, float xR);
  QMatrix4x4 rtY(QMatrix4x4 modelMatrix, float rY, float yR);
  QMatrix4x4 rtZ(QMatrix4x4 modelMatrix, float rZ, float zR);

  QMatrix4x4 scl(QMatrix4x4 modelMatrix, float scale, float scaleModel);
  QPair<long, long> getInfo();

 private:
  Controller() {}
  ~Controller() {}
  Controller(Controller const &) = delete;
  Controller &operator=(Controller const &) = delete;
  MatrixAdapter &m_adapter = MatrixAdapter::Instance();
  QPair<long, long> rez;
};
}  // namespace s21
#endif  //  SRC_CONTROLLER_CONTROLLER_H_
