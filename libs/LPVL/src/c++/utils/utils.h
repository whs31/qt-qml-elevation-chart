//
// Created by whs31 on 11.08.23.
//

#pragma once
#include <QtQuick/QSGGeometryNode>

namespace LPVL::utils
{
  QSGGeometryNode* createSimpleGeometryNode(const QColor& color, unsigned int drawing_mode, float line_width = 1);
} // LPVL::utils
