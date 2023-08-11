//
// Created by whs31 on 11.08.23.
//

#pragma once
#include <QtCore/QMetaType>

namespace ElevationChart
{
  class Metrics
  {
    Q_GADGET
    Q_PROPERTY(float rateOfClimb READ rateOfClimb WRITE setRateOfClimb FINAL)
    Q_PROPERTY(float rateOfDescend READ rateOfDescend WRITE setRateOfDescend FINAL)
    Q_PROPERTY(float fallbackVelocity READ fallbackVelocity WRITE setFallbackVelocity FINAL)

    public:
      Metrics();
      Metrics(float roc, float rod, float vh);

      [[nodiscard]] float rateOfClimb() const;      void setRateOfClimb(float);
      [[nodiscard]] float rateOfDescend() const;    void setRateOfDescend(float);
      [[nodiscard]] float fallbackVelocity() const; void setFallbackVelocity(float);

    private:
      float m_rateOfClimb;
      float m_rateOfDescend;
      float m_fallbackVelocity;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::Metrics)

namespace ElevationChart
{
  inline Metrics::Metrics()
    : m_rateOfClimb(10.0f)
    , m_rateOfDescend(10.0f)
    , m_fallbackVelocity(10.0f)
  {}

  inline Metrics::Metrics(float roc, float rod, float vh)
    : m_rateOfClimb(roc)
    , m_rateOfDescend(rod)
    , m_fallbackVelocity(vh)
  {}

  inline float Metrics::rateOfClimb() const { return m_rateOfClimb; }
  inline void Metrics::setRateOfClimb(float x) { m_rateOfClimb = x; }

  inline float Metrics::rateOfDescend() const { return m_rateOfDescend; }
  inline void Metrics::setRateOfDescend(float x) { m_rateOfDescend = x; }

  inline float Metrics::fallbackVelocity() const { return m_fallbackVelocity; }
  inline void Metrics::setFallbackVelocity(float x) { m_fallbackVelocity = x; }
} // ElevationChart