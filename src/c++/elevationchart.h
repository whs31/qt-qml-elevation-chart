//
// Created by whs31 on 03.08.23.
//

#pragma once

#include <memory>
#include <QtQuick/QQuickItem>
#include <QtQuick/QSGGeometry>
#include <SG/BasicPalette>
#include "types/route.h"
#include "provider/randomdataprovider.h"

using std::unique_ptr;

class QSGGeometryNode;

namespace ElevationChart
{
  class ChartItem : public QQuickItem
  {
    Q_OBJECT
    Q_PROPERTY(SG::BasicPalette palette READ palette WRITE setPalette NOTIFY paletteChanged FINAL)
    Q_PROPERTY(ElevationChart::Route route READ route WRITE setRoute NOTIFY routeChanged FINAL)
    Q_PROPERTY(QGeoCoordinate uavPosition READ uavPosition WRITE setUavPosition NOTIFY uavPositionChanged FINAL)

    // metrics gadget

    // envelope gadget

    Q_PROPERTY(bool intersecting READ intersecting WRITE setIntersecting NOTIFY intersectingChanged FINAL)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged FINAL)
    // matching metrics
    // is busy

    public:
      explicit ChartItem(QQuickItem* parent = nullptr);

      [[nodiscard]] SG::BasicPalette palette() const;   void setPalette(SG::BasicPalette);
      [[nodiscard]] Route route() const;                void setRoute(const Route&);
      [[nodiscard]] QGeoCoordinate uavPosition() const; void setUavPosition(const QGeoCoordinate&);

      [[nodiscard]] bool intersecting() const;          void setIntersecting(bool);
      [[nodiscard]] bool valid() const;                 void setValid(bool);

      //Q_INVOKABLE void applyMetricsCorrection() noexcept;
      //Q_INVOKABLE void estimateEnvelope() noexcept;
      //Q_INVOKABLE void applyEnvelopeCorrection() noexcept;

    signals:
      void paletteChanged();
      void intersectingChanged();
      void validChanged();
      void routeChanged();
      void uavPositionChanged();

    protected:
      QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;
      void requireRecolor();
      void fulfillRecolor();

      void updateProfile() noexcept;
      void updateBounds() noexcept;

      void handleBackgroundNode() noexcept;
      void handleProfileNode() noexcept;
      void handleRouteNode() noexcept;

      QSGGeometry::Point2D toPixel(float x, float y, float x_max, float y_max) const;
      float toPixelX(float x, float x_max) const;
      float toPixelY(float y, float y_max) const;

    protected:
      bool m_require_recolor;
      QSGGeometryNode* m_background_node;
      QSGGeometryNode* m_profile_node;
      QSGGeometryNode* m_route_node;

    private:
      struct Bound
      {
        float x_max;
        float y_max;
      };

      SG::BasicPalette m_palette;
      bool m_intersecting;
      bool m_valid;
      Route m_route;
      QGeoCoordinate m_uav_position;

      unique_ptr<RandomDataProvider> m_random_provider;

      vector<ElevationPoint> m_profile;
      Bound m_bound;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::ChartItem*);