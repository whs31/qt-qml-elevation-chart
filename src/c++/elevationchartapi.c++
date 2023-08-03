//
// Created by whs31 on 03.08.23.
//

#include "elevationchartapi.h"
#include "elevationchart.h"
#include <QtCore/QDebug>

namespace ElevationChart
{
  API* API::get() { static API instance; return &instance; }
  API::API(QObject* parent)
    : QObject(parent)
    , m_source(nullptr) // !
  {}

  void API::setSource(ChartItem* item) {
    m_source = item;
    qDebug() << "<elevation-chart> Source set for C++ wrapper:" << item;
  }
  bool API::valid() const { return (m_source != nullptr); }


} // ElevationChart