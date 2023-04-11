#include "cdeclarativemultipolygon.hpp"
#include "../scenegraph/glmultipolygonshader.hpp"

#include <cmath>
#include <QSGGeometryNode>
#include <vector>

using namespace ChartsOpenGL;
using std::vector;

CDeclarativeMultipolygon::CDeclarativeMultipolygon(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    #ifdef QT_DEBUG
        qDebug() << "<charts> CDeclarativeMultipolygon initialized";
    #endif
}

void CDeclarativeMultipolygon::setList(const std::list<QPointF>& points)
{
    m_points = points;
    this->update();
}

void CDeclarativeMultipolygon::asyncAppend(const std::list<QPointF>& points)
{
    for(QPointF point : points)
        m_points.push_back(point);
    this->update();
}

void CDeclarativeMultipolygon::clear()
{
    m_points.clear();
    //this->update();
}

QSGNode* CDeclarativeMultipolygon::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
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
        QSGSimpleMaterial<State>* material = GLMultipolygonShader::createMaterial();
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
    geometry->setDrawingMode(GL_QUADS);
    geometry->setLineWidth(10);

    // создаем вектор точек (Vertex = Point2D, VertexT = TexturedPoint2D)
    // задаем в него точки графика в пиксельных координатах и координаты UV (опционально)
    vector<VertexT> glPoints;
    Vertex max(0, 0);
    for(QPointF point : m_points)
        if(point.y() > max.y)
            max.y = point.y();
    max.x = m_points.back().x();
    max.y *= 1.05;
    bool flip = false;
    for(QPointF point : m_points)
    {
        if(flip)
        {
            glPoints.push_back(VertexT(point.x(), point.y(), point.x() / max.x, point.y() / max.y));
            glPoints.push_back(VertexT(point.x(), height(), point.x() / max.x, height() / max.y));
            flip = !flip;
        }
        else
        {
            glPoints.push_back(VertexT(point.x(), height(), point.x() / max.x, height() / max.y));
            glPoints.push_back(VertexT(point.x(), point.y(), point.x() / max.x, point.y() / max.y));
            flip = !flip;
        }
    }

    if(glPoints.size() % 2 != 0)
        glPoints.push_back(VertexT(m_points.back().x(), height(), m_points.back().x() / max.x, height() / max.y));

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

QString CDeclarativeMultipolygon::fillColor() const { return m_fillColor; }
void CDeclarativeMultipolygon::setFillColor(const QString& col) {
    if (m_fillColor == col) return;
    m_fillColor = col;
    emit fillColorChanged();
}
