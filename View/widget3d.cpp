#include "widget3d.h"

#include <QMouseEvent>
#include <QOpenGLContext>

#include "eye.h"
#include "simpleobject3d.h"
namespace s21 {
Widget3d::Widget3d(QWidget *parent) : QOpenGLWidget(parent) {
  m_Eye = new Eye;
  m_Eye->translate(QVector3D(0.0f, 0.0f, 0.0f));
  m_colorBack = QVector4D(0.0, 0.0, 0.0, 1.0);
  m_projection = 1;
  m_OneObject = nullptr;
  m_width = 0;
  m_height = 0;
  vm = nullptr;
}

Widget3d::~Widget3d() {
  makeCurrent();  // убирает варнинг "QOpenGLTexturePrivate::destroy() called
                  // without a current context"
  helperClean();
  if (m_Eye != nullptr) {
    delete m_Eye;
  }
}

void Widget3d::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  initShaders();
}

void Widget3d::resizeGL(int width, int height) {
  m_width = width;
  m_height = height;
  m_projectionMatrix.setToIdentity();
  chengeProjection();
}

void Widget3d::paintGL() {
  glClearColor(m_colorBack.x(), m_colorBack.y(), m_colorBack.z(), m_colorBack.w());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_program.bind();
  m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
  m_Eye->draw(&m_program);

  for (auto obj : m_objects) {
    obj->draw(&m_program, context()->functions());
  }

  context()->functions()->glFlush();
  m_program.release();
}

void Widget3d::setProjection(int projection) {
  m_projection = projection;
  resizeGL(m_width, m_height);
}

void Widget3d::chengeProjection() {
  if (m_projection == 1) {
    float aspect = m_width / (float)m_height;
    m_projectionMatrix.perspective(45, aspect, 0.1f, 1000.0f);
  } else if (m_projection == 0) {
    m_projectionMatrix.ortho(-6, 10, -6, 5, -100, 100);
  }
}

void Widget3d::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton ||
      event->buttons() == Qt::RightButton) {
    m_MousePosition = QVector2D(event->position());
  }

  event->accept();
}

void Widget3d::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::LeftButton) {
    QVector2D diff_pos = QVector2D(event->position()) - m_MousePosition;
    m_MousePosition = QVector2D(event->position());
    float angle = diff_pos.length() / 2.0;
    QVector3D axis = QVector3D(diff_pos.y(), diff_pos.x(), 0.0);
    m_Eye->rotate(QQuaternion::fromAxisAndAngle(axis, angle));
  }
  if (event->buttons() == Qt::RightButton) {
    QVector2D diff_pos2 =
        (QVector2D(event->position()) - m_MousePosition) / 150;
    m_MousePosition = QVector2D(event->position());
    m_Eye->translate(QVector3D(diff_pos2.x(), -diff_pos2.y(), 0.0));
  }

  update();
  event->accept();
}

void Widget3d::wheelEvent(QWheelEvent *event) {
  if (event->angleDelta().y() > 0) {
    m_Eye->translate(QVector3D(0.0f, 0.0f, 0.25f));
  } else if (event->angleDelta().y() < 0) {
    m_Eye->translate(QVector3D(0.0f, 0.0f, -0.25f));
  }

  update();
}

void Widget3d::initShaders() {
  if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/View/shaders/vshader.vsh"))
    close();
  if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/View/shaders/fshader.fsh"))
    close();
  if (!m_program.link()) close();
}

void Widget3d::helperClean() {
  for (auto o : m_objects) delete o;
  m_objects.clear();
}

bool Widget3d::addNewModel(const QString &pathModel) {
  m_OneObject = m_control.loadObjectModel(pathModel);
  return m_OneObject != nullptr;
}

void Widget3d::LoadNewModel() {
  if (m_OneObject) {
    helperClean();
    m_objects.append(m_OneObject);
    m_OneObject = nullptr;
  }
  update();
}

void Widget3d::deletModel() {
  helperClean();
  update();
}

QVector<SimpleObject3D *> &Widget3d::getm_Oobjects() { return m_objects; }

void Widget3d::setColorBack(QVector4D colorB) { m_colorBack = colorB; }

Eye *Widget3d::getEye() { return m_Eye; }

void Widget3d::changeEye() { m_Eye->changeMatrix(); }
}  // namespace s21
