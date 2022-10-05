#ifndef SRC_VIEW_MEMENTO_H_
#define SRC_VIEW_MEMENTO_H_
#include <QColor>
namespace s21 {
class Memento {
 public:
  Memento();
  Memento(QColor backgroundColor, QColor surfaceColor, QColor linesColor,
          QColor verticesColor, int vertexcesForm, int vertexcesSize,
          int lineForm, int lineThickness, int projectionType);
  void setBackgroundColor(const QColor &newBackgroundColor);
  const QColor &BackgroundColor() const;
  void setSurfaceColor(const QColor &newSurfaceColor);
  const QColor &SurfaceColor() const;
  void setLinessColor(const QColor &newLinessColor);
  const QColor &LinessColor() const;
  void setVerticesColor(const QColor &newVerticesColor);
  const QColor &VerticesColor() const;
  void setVertexcesForm(int newVertexcesForm);
  int VertexcesForm() const;
  void setVertexcesSize(int newVertexcesSize);
  int VertexcesSize() const;
  void setLineForm(int newLineForm);
  int LineForm() const;
  void setLineThickness(int newLineThickness);
  int LineThickness() const;
  void setProjectionType(int newProjectionType);
  int ProjectionType() const;

 private:
  QColor m_BackgroundColor;
  QColor m_SurfaceColor;
  QColor m_LinessColor;
  QColor m_VerticesColor;
  int m_VertexcesForm;
  int m_VertexcesSize;
  int m_LineForm;
  int m_LineThickness;
  int m_ProjectionType;
};
}  // namespace s21
#endif  //  SRC_VIEW_MEMENTO_H_
