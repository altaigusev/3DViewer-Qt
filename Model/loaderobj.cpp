#include "loaderobj.h"

#include <QFile>
#include <QRegularExpression>

namespace s21 {
LoaderObj::LoaderObj() {
  countVertexces = 0;
  countEdge = 0;
}

QPair<QVector<VertexData>, QVector<GLuint>> LoaderObj::loadModel(
    const QString &pathModel) {
  std::ifstream input(pathModel.toStdString());

  float x;
  float y;
  float z;
  float tex1;
  float tex2;
  float normals1;
  float normals2;
  float normals3;

  if (!input) {
    return rez;
  }
  while (input) {
    input >> type;
    if (type == "v") {
      ++countVertexces;
      input >> x >> y >> z;
      coords.append(QVector3D(x, y, z));
    } else if (type == "vt") {
      input >> tex1 >> tex2;
      texCooords.append(QVector2D(tex1, tex2));
    } else if (type == "vn") {
      input >> normals1 >> normals2 >> normals3;
      normals.append(QVector3D(normals1, normals2, normals3));
    } else if (type == "f") {
        ++countEdge;
      std::string tempString;
      std::string fLine;

      getline(input, tempString);
      std::stringstream fStream(tempString);

      while (fStream >> fLine) {
        std::stringstream stream2(fLine);

        if (getline(stream2, tempString, '/')) {
          vertexes.append(VertexData(coords[std::stol(tempString) - 1]));
          indexes.append(indexes.size());

          if (getline(stream2, tempString, '/') && tempString != "") {
            vertexes.back().texCoord = texCooords[std::stol(tempString) - 1];

            if (getline(stream2, tempString, '/')) {
              vertexes.back().normal = normals[std::stol(tempString) - 1];
            }
          } else if (*tempString.begin() == '/') {
            vertexes.back().normal = normals[std::stol(tempString) - 1];
          }
        }
      }
    }
  }
  countEdge = countEdge > 0 ? countEdge - 1 : countEdge;
  input.close();
  rez = {vertexes, indexes};
  return rez;
}

QPair<long, long> LoaderObj::getCountVF() {
  return QPair<long, long>{countVertexces, countEdge};
}

}  //  namespace s21
