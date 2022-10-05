#ifndef SRC_VIEW_SIMPLEOBJECT3D_H_
#define SRC_VIEW_SIMPLEOBJECT3D_H_
#define GL_SILENCE_DEPRECATION
#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QVector2D>

#include "Controller/controller.h"

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

namespace s21 {
class SimpleObject3D {
 public:
  SimpleObject3D();
  SimpleObject3D(QVector<s21::VertexData> vertData, QVector<GLuint> indexes);
  ~SimpleObject3D();
  void init(QVector<s21::VertexData> vertData, QVector<GLuint> indexes);
  void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
  void initTexture(const QString &pathTexture);
  void delTexture();
  void pointsFoo(float pointS, QColor colorPoints, int smooth);
  void lineWidth(int l_Width);
  void lineDash(int l_Dash);
  void setColorLine(QVector4D colorM);
  void setColorDot(QVector4D colorD);
  void setColorSurf(QVector4D colorS);
  void setForm(int f);
  void setPointSize(int p);

  void translateModelX(float x);
  void translateModelY(float y);
  void translateModelZ(float z);

  void rotateModelX(float x);
  void rotateModelY(float y);
  void rotateModelZ(float z);
  void scaleModel(float s);
  void changeMatrix();

 protected:
  void free();

 private:
  void changePointSmoothAndSize();
  void changeLineWidthAndDash();
  QOpenGLBuffer m_vertexBuffer;  //  Буффер в котором хранятся координаты вершин
  QOpenGLBuffer m_indexBuffer;  // Буффер с указателями на данные внутри вершинного
                      // буфера, позволяет исключить дублирование вершин
  QMatrix4x4 m_modelMatrix;  //  Матрица Модели

  QOpenGLTexture *m_texture;  //  указатель на текстуру
  float m_PointSize;
  int m_pointSmoth;
  QVector4D m_colorModel;
  QVector4D m_colorDot;
  QVector4D m_colorSurf;
  int m_linesWidth;
  int m_linesDashed;
  int m_form;

  QQuaternion m_rotateModel;
  QVector3D m_TranslateModel;

  float m_x;
  float m_y;
  float m_z;

  float m_rX;
  float m_rY;
  float m_rZ;

  float m_scale;
  QMatrix4x4 m_modelMatrixFirst;

  s21::Controller &m_control = s21::Controller::Instance();
};
}  // namespace s21
#endif  //  SRC_VIEW_SIMPLEOBJECT3D_H_
