//
// Created by whs31 on 03.08.23.
//

#pragma once
#include <QtCore/QObject>

namespace ElevationChart
{
  class ChartItem;
  class API : public QObject
  {
    Q_OBJECT

    public:
      static API* get();

      Q_INVOKABLE void setSource(ChartItem* item);
      [[nodiscard]] bool valid() const;

    private:
      explicit API(QObject* parent = nullptr);
      API(const API&);
      API& operator=(const API&);

    private:
      ChartItem* m_source;
  };
} // ElevationChart
