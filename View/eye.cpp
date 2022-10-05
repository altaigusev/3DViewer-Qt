#include "eye.h"
namespace s21 {
Eye::Eye() {
  defaultSettigs();
  m_ViewMatrixFirst = m_ViewMatrix;
  m_flatShadow = false;
}

void Eye::draw(QOpenGLShaderProgram *program,
               const QOpenGLFunctions *functions) {
  if (functions != nullptr) return;

  m_ViewMatrix.setToIdentity();
  m_ViewMatrix.translate(m_Translate);
  m_ViewMatrix.rotate(m_Rotate);

  program->setUniformValue("u_lightPosition", m_lightPosition);
  program->setUniformValue("u_lightPower", m_lightPower);
  program->setUniformValue("shading", m_flatShadow);
  program->setUniformValue("u_colorRed", m_colorRed);
  program->setUniformValue("u_colorGreen", m_colorGreen);
  program->setUniformValue("u_colorBlue", m_colorBlue);
  program->setUniformValue("u_viewMatrix", m_ViewMatrix);
}

void Eye::rotate(const QQuaternion &r) { m_Rotate = r * m_Rotate; }

void Eye::translate(const QVector3D &t) { m_Translate += t; }

void Eye::setLightPosition(float positionX, float positionY) {
  m_lightPosition = QVector4D(positionX, positionY, 0.0f, 1.0f);
}

void Eye::setLightPower(float lPower) { m_lightPower = lPower; }

void Eye::lightOnOff(bool on) {
    m_lightPower = !on ? 0.0f : 5.0f;
}

void Eye::flatShadingOn(bool flat) {
    m_flatShadow = flat;
}

void Eye::setColorLightRed(float lRed) { m_colorRed = lRed; }

void Eye::setColorLightGreen(float lGreen) { m_colorGreen = lGreen; }

void Eye::setColorLightBlue(float lBlue) { m_colorBlue = lBlue; }

void Eye::changeMatrix() {
  defaultSettigs();
  m_ViewMatrix.setToIdentity();
  m_ViewMatrix.translate(m_Translate);
  m_ViewMatrix.rotate(m_Rotate);
}

void Eye::defaultSettigs() {
  m_lightPosition = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
  m_lightPower = 5.0f;
  m_colorRed = 1.0f;
  m_colorGreen = 1.0f;
  m_colorBlue = 1.0f;
  m_Translate = QVector3D(0.0f, 0.0f, -5.0f);
  m_Rotate = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 0.0f), 0.0);
}
}  // namespace s21
