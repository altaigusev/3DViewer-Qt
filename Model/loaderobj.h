#ifndef SRC_MODEL_LOADEROBJ_H_
#define SRC_MODEL_LOADEROBJ_H_

#include <QList>
#include <QOpenGLBuffer>
#include <QTextStream>
#include <QVector2D>
#include <fstream>
#include <sstream>

namespace s21 {
struct VertexData {
  VertexData() {}
  VertexData(QVector3D pos, QVector2D tex, QVector3D nor)
      : position(pos), texCoord(tex), normal(nor) {}

  VertexData(QVector3D pos, QVector2D tex)
      : position(pos), texCoord(tex), normal(QVector3D(0.0, 0.0, 0.0)) {}

  VertexData(QVector3D pos, QVector3D nor)
      : position(pos), texCoord(QVector2D(0.0, 0.0)), normal(nor) {}

  explicit VertexData(QVector3D pos)
      : position(pos),
        texCoord(QVector2D(0.0, 0.0)),
        normal(QVector3D(0.0, 0.0, 0.0)) {}

  QVector3D position;  //  координаты точки
  QVector2D texCoord;  //  текстурные координаты
  QVector3D normal;    //  координаты точки для нормали
};
class LoaderObj {
 public:
  LoaderObj();
  QPair<QVector<VertexData>, QVector<GLuint>> loadModel(
      const QString &pathModel);
  QPair<long, long> getCountVF();

 private:
  QVector<QVector3D> coords;
  QVector<QVector2D> texCooords;
  QVector<QVector3D> normals;
  QVector<VertexData> vertexes;
  QVector<GLuint> indexes;
  std::string type;
  QPair<QVector<VertexData>, QVector<GLuint>> rez;
  long countVertexces;
  long countEdge;
};
}  // namespace s21
#endif  //  SRC_MODEL_LOADEROBJ_H_
