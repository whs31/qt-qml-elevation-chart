/* ---------------------------------------------------------------------
 * LPVL - Linear Algebra, Plotting and Visualisation Library
 * Copyright (C) 2023 whs31.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 * http://www.gnu.org/licenses.
 *
 * github.com/whs31/lpvl
 * ---------------------------------------------------------------------- */

#pragma once
#include <QtQuick/QSGGeometryNode>

namespace LPVL::utils
{
  QSGGeometryNode* createSimpleGeometryNode(const QColor& color, unsigned int drawing_mode, float line_width = 1);
} // LPVL::utils
