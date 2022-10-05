#include "simpleobject3d.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
namespace s21 {
SimpleObject3D::SimpleObject3D()
    : m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_colorModel(QVector4D(0.0, 0.0, 0.0, 0.0)),
      m_colorDot(QVector4D(0.0, 0.0, 0.0, 0.0)),
      m_colorSurf(QVector4D(0.0, 0.0, 0.0, 0.0)) {
  m_texture = nullptr;
  m_pointSmoth = 0;
  m_PointSize = 0;
  m_linesWidth = 3;
  m_linesDashed = 0;
  m_form = GL_LINES;
  m_x = 0.0f;
  m_y = 0.0f;
  m_z = 0.0f;
  m_rX = 0.0f;
  m_rY = 0.0f;
  m_rZ = 0.0f;
  m_scale = 0.0f;
  m_modelMatrixFirst = m_modelMatrix;
}

SimpleObject3D::SimpleObject3D(QVector<s21::VertexData> vertData, QVector<GLuint> indexes)
    : m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer) {
  m_texture = nullptr;
  init(vertData, indexes);
}

SimpleObject3D::~SimpleObject3D() { free(); }

void SimpleObject3D::free() {
  if (m_vertexBuffer.isCreated()) m_vertexBuffer.destroy();
  if (m_indexBuffer.isCreated()) m_indexBuffer.destroy();
  if (m_texture != nullptr && m_texture->isCreated()) {
    delete m_texture;
    m_texture = nullptr;
  }
}

void SimpleObject3D::init(QVector<s21::VertexData> vertData, QVector<GLuint> indexes) {
  free();
  m_vertexBuffer.create();  // создание буффера вершин
  m_vertexBuffer.bind();  // размещение его в видеопамять
  m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(s21::VertexData));
  m_vertexBuffer.release();
  m_indexBuffer.create();
  m_indexBuffer.bind();
  m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
  m_indexBuffer.release();
  m_modelMatrix.setToIdentity();
  m_colorModel = QVector4D(0.0, 0.0, 0.0, 0.0);
  m_colorDot = QVector4D(0.0, 0.0, 0.0, 0.0);
  m_colorSurf = QVector4D(0.0, 0.0, 0.0, 0.0);
  m_pointSmoth = 0;
  m_PointSize = 0;
  m_linesWidth = 3;
  m_linesDashed = 0;
  m_form = GL_LINES;
  m_x = 0.0f;
  m_y = 0.0f;
  m_z = 0.0f;

  m_rX = 0.0f;
  m_rY = 0.0f;
  m_rZ = 0.0f;

  m_scale = 0.0f;
}

void SimpleObject3D::initTexture(const QString &pathTexture) {
  delTexture();
  m_texture = new QOpenGLTexture(QImage(pathTexture).mirrored());
}

void SimpleObject3D::delTexture() {
  if (m_texture) {
    delete m_texture;
    m_texture = nullptr;
  }
}

void SimpleObject3D::pointsFoo(float pointS, QColor colorPoints, int smooth) {
  m_PointSize = pointS;
  m_colorDot = QVector4D(colorPoints.redF(), colorPoints.greenF(),
                         colorPoints.blueF(), colorPoints.alphaF());
  if (smooth == 0) {
    m_pointSmoth = 0;
  } else if (smooth == 1) {
    m_pointSmoth = 1;
  } else {
    m_pointSmoth = -1;
    m_PointSize = 0;
  }
}

void SimpleObject3D::lineWidth(int l_Width) { m_linesWidth = l_Width; }

void SimpleObject3D::lineDash(int l_Dash) { m_linesDashed = l_Dash; }

void SimpleObject3D::setColorLine(QVector4D colorM) { m_colorModel = colorM; }

void SimpleObject3D::setColorDot(QVector4D colorD) { m_colorDot = colorD; }

void SimpleObject3D::setColorSurf(QVector4D colorS) { m_colorSurf = colorS; }

void SimpleObject3D::setForm(int f) { m_form = f; }

void SimpleObject3D::setPointSize(int p) { m_PointSize = p; }

void SimpleObject3D::changeLineWidthAndDash() {
  glLineWidth(m_linesWidth);
  if (m_linesDashed == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x3333);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
}

void SimpleObject3D::changePointSmoothAndSize() {
  glPointSize(m_PointSize);
  if (m_pointSmoth == 0) {
    glDisable(GL_POINT_SMOOTH);
    glEnable(GL_POINT_SIZE);
  } else if (m_pointSmoth == 1) {
    m_pointSmoth = 1;
    glDisable(GL_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
  } else if (m_pointSmoth == -1) {
    glDisable(GL_POINT_SIZE);
    glDisable(GL_POINT_SMOOTH);
    m_PointSize = 0;
  }
}

void SimpleObject3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) {
  if (!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) return;
  if (m_texture) {
    m_texture->bind(0);  // размещаем текстуры в видеопамяти под нулевым номером
    program->setUniformValue("u_texture", 0);
    // задаём текстуры в шайдер, ноль это номер
    // текстуры которая будет отрисововаться
  }

  program->setUniformValue("u_colorModel", m_colorModel);
  program->setUniformValue("u_modelMatrix", m_modelMatrix);

  m_vertexBuffer.bind();
  int offset = 0;

  int vertLoc = program->attributeLocation("a_position");
  program->enableAttributeArray(vertLoc);
  program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3,
                              sizeof(s21::VertexData));

  offset += sizeof(QVector3D);

  int texloc = program->attributeLocation("a_texcoord");
  program->enableAttributeArray(texloc);
  program->setAttributeBuffer(texloc, GL_FLOAT, offset, 2,
                              sizeof(s21::VertexData));

  offset += sizeof(QVector2D);

  int normLoc = program->attributeLocation("a_normal");
  program->enableAttributeArray(normLoc);
  program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3,
                              sizeof(s21::VertexData));
  m_indexBuffer.bind();

  if (m_form > 0 && !(m_form == GL_TRIANGLES || m_form == GL_POLYGON || m_form == GL_QUADS)) {
    changeLineWidthAndDash();
    functions->glDrawElements(m_form, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
  }

  changePointSmoothAndSize();
  if (m_PointSize > 0) {
    program->setUniformValue("u_colorModel", m_colorDot);
    functions->glDrawElements(GL_POINTS, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
  }

  if (m_form == GL_TRIANGLES || m_form == GL_POLYGON || m_form == GL_QUADS) {
    program->setUniformValue("u_colorModel", m_colorSurf);
    functions->glDrawElements(m_form, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
  }

  m_vertexBuffer.release();
  m_indexBuffer.release();
  if (m_texture) m_texture->release();
  functions->glFlush();
  program->release();
}

void SimpleObject3D::translateModelX(float x) {
  m_modelMatrix = m_control.transX(m_modelMatrix, x);
  m_x = x;
}

void SimpleObject3D::translateModelY(float y) {
  m_modelMatrix = m_control.transY(m_modelMatrix, y);
  m_y = y;
}

void SimpleObject3D::translateModelZ(float z) {
  m_modelMatrix = m_control.transZ(m_modelMatrix, z);
  m_z = z;
}

void SimpleObject3D::rotateModelX(float x) {
  m_modelMatrix = m_control.rtX(m_modelMatrix, m_rX, x);
  m_rX = x;
}

void SimpleObject3D::rotateModelY(float y) {
  m_modelMatrix = m_control.rtY(m_modelMatrix, m_rY, y);
  m_rY = y;
}

void SimpleObject3D::rotateModelZ(float z) {
  m_modelMatrix = m_control.rtZ(m_modelMatrix, m_rZ, z);
  m_rZ = z;
}

void SimpleObject3D::scaleModel(float svector) {
  if (svector == 0.0f) {
    m_modelMatrix = m_modelMatrixFirst;
    translateModelX(m_x);
    translateModelY(m_y);
    translateModelZ(m_z);
    m_modelMatrix = m_control.rtX(m_modelMatrix, 0, m_rX);
    m_modelMatrix = m_control.rtY(m_modelMatrix, 0, m_rY);
    m_modelMatrix = m_control.rtZ(m_modelMatrix, 0, m_rZ);
  } else {
    m_modelMatrix = m_control.scl(m_modelMatrix, m_scale, svector);
  }
  m_scale = svector;
}

void SimpleObject3D::changeMatrix() { m_modelMatrix = m_modelMatrixFirst; }
}  // namespace s21
