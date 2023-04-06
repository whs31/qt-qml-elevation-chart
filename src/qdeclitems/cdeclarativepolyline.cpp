#include "cdeclarativepolyline.hpp"
#include <cmath>
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <vector>

using namespace ChartsOpenGL;

CDeclarativePolyline::CDeclarativePolyline(QQuickItem* parent)
    : QQuickItem{parent}
{
    setFlag(ItemHasContents);
    qDebug() << "<charts> CDeclarativePolyline initialized";
}

void CDeclarativePolyline::setList(const std::list<QPointF>& points)
{
    m_points = points;
    this->update();
}

void CDeclarativePolyline::clear()
{
    m_points.clear();
    this->update();
}

QSGNode* CDeclarativePolyline::updatePaintNode(QSGNode *old_node, UpdatePaintNodeData *update_paint_node_data)
{
    Q_UNUSED(update_paint_node_data);

    QSGGeometry* geometry = nullptr;
    QSGGeometryNode* node = static_cast<QSGGeometryNode*>(old_node);
    if(node == nullptr)
    {
        node = new QSGGeometryNode;
        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(QColor(lineColor()));

        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), m_points.size(), 0, QSGGeometry::UnsignedIntType);
        if(not dashed())
            geometry->setDrawingMode(GL_LINE_STRIP);
        else
        {
            if(dashPattern() == DashPattern::Dashed)
                geometry->setDrawingMode(GL_LINES);
            else
                geometry->setDrawingMode(GL_POINTS);
        }
        geometry->setLineWidth(5);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
    }

    geometry = node->geometry();
    if(not dashed())
        geometry->allocate(m_points.size() + 1);
    vector<QSGGeometry::Point2D> glVec;

    // это пиздец)
    // короче. если аллоцировать память под массив (лист) из N точек, то ничего работать не будет
    // поэтому мы аллоцируем память под N+1 точек и присваиваем нулевой точке значение первой
    size_t index = 1;
    if(not dashed())
        geometry->vertexDataAsPoint2D()[0].set(m_points.front().x(), m_points.front().y());

    if(dashed())
    {
        vector<QPointF> vec { std::begin(m_points), std::end(m_points) };
        for(size_t i = 1; i < vec.size(); ++i)
        {
            QSGGeometry::Point2D p1;
            p1.set(vec.at(i-1).x(), vec.at(i-1).y());
            glVec.push_back(p1);
            float segment_length = sqrt(pow(vec.at(i).x() - vec.at(i-1).x(), 2) + pow(vec.at(i).y() - vec.at(i-1).y(), 2));
            float total_dashes = segment_length / dashInterval();
            float _cos = (vec.at(i).x() - vec.at(i-1).x()) / segment_length;
            float _sin = (vec.at(i).y() - vec.at(i-1).y()) / segment_length;
            for(size_t j = 1; j <= total_dashes; ++j)
            {
                QPointF dash(vec.at(i-1).x() + dashInterval() * j * _cos, vec.at(i-1).y() + dashInterval() * j * _sin);
                QSGGeometry::Point2D p2;
                p2.set(dash.x(), dash.y());
                glVec.push_back(p2);
            }
        }
        QSGGeometry::Point2D p3;
        p3.set(m_points.back().x(), m_points.back().y());
        glVec.push_back(p3);

        if(not glVec.empty())
        {
            geometry->allocate(glVec.size() + 1);

            for(size_t i = 1; i < glVec.size(); ++i)
                geometry->vertexDataAsPoint2D()[i].set(glVec.at(i).x, glVec.at(i).y);
        }
    }
    else
        for(QPointF point : m_points)
            geometry->vertexDataAsPoint2D()[index++].set(point.x(), point.y());

    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

QString CDeclarativePolyline::lineColor() const { return m_lineColor; }
void CDeclarativePolyline::setLineColor(const QString& col) {
    if (m_lineColor == col) return;
    m_lineColor = col;
    emit lineColorChanged();
}

bool CDeclarativePolyline::dashed() const { return m_dashed; }
void CDeclarativePolyline::setDashed(bool state) {
    if (m_dashed == state) return;
    m_dashed = state;
    emit dashedChanged();
}

float CDeclarativePolyline::dashInterval() const { return m_dashInterval; }
void CDeclarativePolyline::setDashInterval(float interval) {
    if (qFuzzyCompare(m_dashInterval, interval)) return;
    m_dashInterval = interval;
    emit dashIntervalChanged();
}

int CDeclarativePolyline::dashPattern() const { return m_dashPattern; }
void CDeclarativePolyline::setDashPattern(const int& pattern) {
    if (m_dashPattern == pattern) return;
    m_dashPattern = pattern;
    emit dashPatternChanged();
}
