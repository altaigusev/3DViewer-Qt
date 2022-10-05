#ifndef SRC_VIEW_WIDGET3D_H_
#define SRC_VIEW_WIDGET3D_H_

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include "Controller/controller.h"
namespace s21 {

class SimpleObject3D;
class Eye;
class ViewerMemento;

class Widget3d : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit Widget3d(QWidget *parent = 0);
  ~Widget3d();
  bool addNewModel(const QString &pathModel);
  void LoadNewModel();
  void deletModel();
  void setColorBack(QVector4D colorB);
  void setProjection(int projection);
  Eye *getEye();
  void changeEye();
  QVector<SimpleObject3D *> &getm_Oobjects();

 protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void initShaders();
  void helperClean();
  void chengeProjection();

 private:
  QMatrix4x4 m_projectionMatrix;
  QOpenGLShaderProgram m_program;
  QVector2D m_MousePosition;
  QVector<SimpleObject3D *> m_objects;
  SimpleObject3D *m_OneObject;
  Eye *m_Eye;
  s21::Controller &m_control = s21::Controller::Instance();
  QVector4D m_colorBack;
  int m_projection;
  int m_width;
  int m_height;
  ViewerMemento *vm;
};
}  // namespace s21
#endif  // SRC_VIEW_WIDGET3D_H_
