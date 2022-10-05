#include "controller.h"

#include "../Model/affineTransformations.h"
#include "../Model/s21_matrix_oop.h"
#include "../View/eye.h"
#include "../View/simpleobject3d.h"

namespace s21 {
SimpleObject3D *Controller::loadObjectModel(const QString &path) {
  LoaderObj loader;
  QPair<QVector<VertexData>, QVector<GLuint>> r = loader.loadModel(path);
  SimpleObject3D *obj;
  if (r.first.size() == 0) {
    obj = nullptr;
  } else {
    obj = new SimpleObject3D(r.first, r.second);
  }
  rez = loader.getCountVF();
  return obj;
}

QMatrix4x4 Controller::transX(QMatrix4x4 modelMatrix, float xM) {
  S21Matrix matrix = m_adapter.returnS21matrix(modelMatrix);
  translateX(&matrix, xM);
  return m_adapter.returnQmatrix4x4(matrix);
}

QMatrix4x4 Controller::transY(QMatrix4x4 modelMatrix, float yM) {
  S21Matrix matrix = m_adapter.returnS21matrix(modelMatrix);
  translateY(&matrix, yM);
  return m_adapter.returnQmatrix4x4(matrix);
}

QMatrix4x4 Controller::transZ(QMatrix4x4 modelMatrix, float zM) {
  S21Matrix matrix = m_adapter.returnS21matrix(modelMatrix);
  translateZ(&matrix, zM);
  return m_adapter.returnQmatrix4x4(matrix);
}

QMatrix4x4 Controller::rtX(QMatrix4x4 modelMatrix, float rX, float xR) {
  S21Matrix matrix = m_adapter.returnS21matrix(modelMatrix);
  rotateX(&matrix, -(rX - xR));
  return m_adapter.returnQmatrix4x4(matrix);
}

QMatrix4x4 Controller::rtY(QMatrix4x4 modelMatrix, float rY, float yR) {
  S21Matrix matrix = m_adapter.returnS21matrix(modelMatrix);
  rotateY(&matrix, -(rY - yR));
  return m_adapter.returnQmatrix4x4(matrix);
}

QMatrix4x4 Controller::rtZ(QMatrix4x4 modelMatrix, float rZ, float zR) {
  S21Matrix matrix = m_adapter.returnS21matrix(modelMatrix);
  rotateZ(&matrix, -(rZ - zR));
  return m_adapter.returnQmatrix4x4(matrix);
}

QMatrix4x4 Controller::scl(QMatrix4x4 modelMatrix, float sc, float scaleModel) {
  float z = (sc - scaleModel) + 1.0f;
  S21Matrix matrix = m_adapter.returnS21matrix(modelMatrix);
  scale(&matrix, z);
  return m_adapter.returnQmatrix4x4(matrix);
}

QPair<long, long> Controller::getInfo() { return rez; }
}  //  namespace s21
