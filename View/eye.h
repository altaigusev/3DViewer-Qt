#ifndef SRC_VIEW_EYE_H_
#define SRC_VIEW_EYE_H_

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QQuaternion>
#include <QVector3D>
namespace s21 {

class Eye {
 public:
  Eye();
  void draw(QOpenGLShaderProgram *program,
            const QOpenGLFunctions *functions = nullptr);
  void rotate(const QQuaternion &r);
  // QQuaternion представляет собой объект, который содержит информацию о
  // векторе вокруг которого осуществляется поворот и об угле поворота
  void translate(const QVector3D &t);
  void setLightPosition(float positionX, float positionY);
  void setLightPower(float lPower);
  void lightOnOff(bool on);
  void flatShadingOn(bool flat);
  void setColorLightRed(float lRed);
  void setColorLightGreen(float lGreen);
  void setColorLightBlue(float lBlue);
  void changeMatrix();

 private:
  void defaultSettigs();
  QQuaternion m_Rotate;
  QVector3D m_Translate;
  QMatrix4x4 m_ViewMatrix;
  QMatrix4x4 m_ViewMatrixFirst;

  QVector4D m_lightPosition;
  float m_lightPower;
  float m_colorRed;
  float m_colorGreen;
  float m_colorBlue;
  bool m_flatShadow;
};

}  // namespace s21
#endif  // SRC_VIEW_EYE_H_
