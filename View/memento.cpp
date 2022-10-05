#include "memento.h"
namespace s21 {
Memento::Memento() {}

Memento::Memento(QColor backgroundColor, QColor surfaceColor, QColor linesColor,
                 QColor verticesColor, int vertexcesForm, int vertexcesSize,
                 int lineForm, int lineThickness, int projectionType)
    : m_BackgroundColor(backgroundColor),
      m_SurfaceColor(surfaceColor),
      m_LinessColor(linesColor),
      m_VerticesColor(verticesColor),
      m_VertexcesForm(vertexcesForm),
      m_VertexcesSize(vertexcesSize),
      m_LineForm(lineForm),
      m_LineThickness(lineThickness),
      m_ProjectionType(projectionType) {}

void Memento::setBackgroundColor(const QColor &newBackgroundColor) {
  m_BackgroundColor = newBackgroundColor;
}

const QColor &Memento::BackgroundColor() const { return m_BackgroundColor; }

void Memento::setSurfaceColor(const QColor &newSurfaceColor) {
  m_SurfaceColor = newSurfaceColor;
}

const QColor &Memento::SurfaceColor() const { return m_SurfaceColor; }

void Memento::setLinessColor(const QColor &newLinessColor) {
  m_LinessColor = newLinessColor;
}

const QColor &Memento::LinessColor() const { return m_LinessColor; }

void Memento::setVerticesColor(const QColor &newVerticesColor) {
  m_VerticesColor = newVerticesColor;
}

const QColor &Memento::VerticesColor() const { return m_VerticesColor; }

void Memento::setVertexcesForm(int newVertexcesForm) {
  m_VertexcesForm = newVertexcesForm;
}

int Memento::VertexcesForm() const { return m_VertexcesForm; }

void Memento::setVertexcesSize(int newVertexcesSize) {
  m_VertexcesSize = newVertexcesSize;
}

int Memento::VertexcesSize() const { return m_VertexcesSize; }

void Memento::setLineForm(int newLineForm) { m_LineForm = newLineForm; }

int Memento::LineForm() const { return m_LineForm; }

void Memento::setLineThickness(int newLineThickness) {
  m_LineThickness = newLineThickness;
}

int Memento::LineThickness() const { return m_LineThickness; }

void Memento::setProjectionType(int newProjectionType) {
  m_ProjectionType = newProjectionType;
}

int Memento::ProjectionType() const { return m_ProjectionType; }
}  // namespace s21
