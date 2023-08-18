//
// Created by whs31 on 11.08.23.
//

#include "utils.h"
#include <QtQuick/QSGFlatColorMaterial>

namespace LPVL::utils
{
  /**
   * \brief Создает новую ноду геометрии с простым материалом с указанными параметрами.
   * \param color - цвет отрисовки ноды.
   * \param drawing_mode - тип отрисовки ноды. См. QSGGeometry::DrawingMode.
   * \param line_width - толщина линии. <i>По умолчанию равно 1.</i>
   * \return Указатель на созданную ноду.
   */
  QSGGeometryNode* createSimpleGeometryNode(const QColor& color, unsigned int drawing_mode, float line_width)
  {
    auto ret = new QSGGeometryNode;
    auto* material = new QSGFlatColorMaterial;
    material->setColor(color);
    ret->setMaterial(material);
    auto* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0, QSGGeometry::UnsignedIntType);
    geometry->setDrawingMode(drawing_mode);
    geometry->setLineWidth(line_width);
    ret->setGeometry(geometry);
    ret->setFlags(QSGNode::OwnsGeometry | QSGNode::OwnsMaterial);

    return ret;
  }
} // LPVL::utils