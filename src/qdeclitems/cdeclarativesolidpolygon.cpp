#include "cdeclarativesolidpolygon.hpp"
#include "../scenegraph/glpolygonshader.hpp"
#include "../scenegraph/glvertextype.hpp"

#include <cmath>
#include <QSGGeometryNode>
#include <vector>

using namespace ChartsOpenGL;
using std::vector;

CDeclarativeSolidPolygon::CDeclarativeSolidPolygon(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    #ifdef QT_DEBUG
        qDebug() << "<charts> CDeclarativePolygon initialized";
    #endif
}

void CDeclarativeSolidPolygon::setList(const std::list<QPointF>& points)
{
    m_points = points;
    this->update();
}

void CDeclarativeSolidPolygon::asyncAppend(const std::list<QPointF>& points)
{
    for(QPointF point : points)
        m_points.push_back(point);
    this->update();
}

void CDeclarativeSolidPolygon::clear()
{
    m_points.clear();
    this->update();
}

QSGNode* CDeclarativeSolidPolygon::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
{
    Q_UNUSED(update_paint_node_data);

    // создаем указатели на саму ноду и на ее геометрию
    QSGGeometry* geometry = nullptr;
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(old_node);

    // сетапим ноду при инстанциировании объекта в кумээль
    if(node == nullptr)
    {
        node = new QSGGeometryNode;

        // это лучше скопипастить
        // ремарка: материалы, использующие UV (fragment shader), должны использовать VertexT
        // в качестве точек ноды. Остальные более простые материалы (напр. flatcolormat) можно
        // ассоциировать с более простым типом Vertex.
        QSGSimpleMaterial<State>* material = GLPolygonShader::createMaterial();
        material->setFlag(QSGMaterial::Blending);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        static_cast<QSGSimpleMaterial<State>*>(node->material())->state()->color = QColor(m_fillColor);
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), m_points.size(), 0, QSGGeometry::UnsignedIntType);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
    }

    // ставим геометрии параметры отрисовки
    geometry = node->geometry();                                                          
    geometry->setDrawingMode(GL_QUAD_STRIP); // GL_QUAD_STRIP
    geometry->setLineWidth(5);

    // создаем вектор точек (Vertex = Point2D, VertexT = TexturedPoint2D)
    // задаем в него точки графика в пиксельных координатах и координаты UV (опционально)
    vector<VertexT> glPoints;
    Vertex max(0, 0);
    for(QPointF point : m_points)
        if(point.y() > max.y)
            max.y = point.y();
    max.x = m_points.back().x();

    bool flip = false;
    for(QPointF point : m_points)
    {
        if(flip)
        {
            glPoints.push_back(VertexT(point.x(), point.y(), point.x() / max.x, point.y() / max.y));
            if(m_loopmode == LoopMode::LoopByItemRect)
                glPoints.push_back(VertexT(point.x(), height(), point.x() / max.x, height() / max.y));
            flip = !flip;
        }
        else
        {
            if(m_loopmode == LoopMode::LoopByItemRect)
                glPoints.push_back(VertexT(point.x(), height(), point.x() / max.x, height() / max.y));
            glPoints.push_back(VertexT(point.x(), point.y(), point.x() / max.x, point.y() / max.y));
            flip = !flip;
        }
    }

    // после создания всех точек аллоцируем память под этот вектор + 1 точку
    geometry->allocate(glPoints.size());

    // задаем в геометрию графа эти точки
    for(size_t i = 0; i < glPoints.size(); ++i)
        geometry->vertexDataAsTexturedPoint2D()[i].set(glPoints.at(i).x, glPoints.at(i).y, glPoints.at(i).u, glPoints.at(i).v);

    // говорим куэмэлю что ему надо задуматься о перерисовке графика
    node->markDirty(QSGNode::DirtyGeometry);

    // не забываем вернуть ноду обратно в граф сцены
    return node;
}

QString CDeclarativeSolidPolygon::fillColor() const { return m_fillColor; }
void CDeclarativeSolidPolygon::setFillColor(const QString& col) {
    if (m_fillColor == col) return;
    m_fillColor = col;
    emit fillColorChanged();
}

void ChartsOpenGL::CDeclarativeSolidPolygon::setLoopMode(LoopMode mode)
{
    m_loopmode = mode;
}

QString CDeclarativeSolidPolygon::drawingMode() const { return m_drawingMode; }
void CDeclarativeSolidPolygon::setDrawingMode(const QString& mode) {
    if (m_drawingMode == mode) return;
    m_drawingMode = mode;
    if(mode == "Points")
        m_glDrawMode == GL_POINTS;
    else if(mode == "Line strip")
        m_glDrawMode == GL_LINE_STRIP;
    else if(mode == "Quad strip")
        m_glDrawMode == GL_QUAD_STRIP;
    else if(mode == "Triangle strip")
        m_glDrawMode == GL_TRIANGLE_STRIP;
    else
        m_glDrawMode == GL_TRIANGLE_FAN;
    emit drawingModeChanged();
}
