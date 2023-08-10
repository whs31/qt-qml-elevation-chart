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
#include "internal/routemodel.h"

using std::unique_ptr;

class QSGGeometryNode;

namespace ElevationChart
{
  class ChartItem : public QQuickItem
  {
    Q_OBJECT
    Q_PROPERTY(SG::BasicPalette palette READ palette WRITE setPalette NOTIFY paletteChanged FINAL)
    Q_PROPERTY(ElevationChart::Route route READ route WRITE setRoute NOTIFY routeChanged FINAL)
    Q_PROPERTY(ElevationChart::RouteModel* model READ model CONSTANT)
    Q_PROPERTY(QGeoCoordinate uavPosition READ uavPosition WRITE setUavPosition NOTIFY uavPositionChanged FINAL)

    // metrics gadget

    // envelope gadget

    Q_PROPERTY(bool intersecting READ intersecting WRITE setIntersecting NOTIFY intersectingChanged FINAL)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged FINAL)
    // matching metrics
    // is busy

    Q_PROPERTY(int shrinkMode READ shrinkMode WRITE setShrinkMode NOTIFY shrinkModeChanged FINAL)

    constexpr static const float ROUTE_LINE_WIDTH = 5.0f;
    constexpr static const float STRETCH = 1.15f;

    struct Bound
    {
      float x_max;
      float y_max;
    };

    public:
      enum ShrinkMode
      {
        ShrinkToProfileHeight,
        ShrinkToRouteHeight
      };
      Q_ENUM(ShrinkMode)

      explicit ChartItem(QQuickItem* parent = nullptr);

      [[nodiscard]] SG::BasicPalette palette() const;   void setPalette(SG::BasicPalette);
      [[nodiscard]] Route route() const;                void setRoute(const Route&);
      [[nodiscard]] RouteModel* model() const;
      [[nodiscard]] QGeoCoordinate uavPosition() const; void setUavPosition(const QGeoCoordinate&);

      [[nodiscard]] bool intersecting() const;          void setIntersecting(bool);
      [[nodiscard]] bool valid() const;                 void setValid(bool);

      [[nodiscard]] int shrinkMode() const;             void setShrinkMode(int);

      //Q_INVOKABLE void applyMetricsCorrection() noexcept;
      //Q_INVOKABLE void estimateEnvelope() noexcept;
      //Q_INVOKABLE void applyEnvelopeCorrection() noexcept;

    signals:
      void paletteChanged();
      void intersectingChanged();
      void validChanged();
      void routeChanged();
      void uavPositionChanged();
      void shrinkModeChanged();

    protected:
      QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;
      void requireRecolor();
      void fulfillRecolor();
      [[nodiscard]] Bound& bounds();

      void updateProfile() noexcept;
      void updateBounds() noexcept;

      void handleBackgroundNode() noexcept;
      void handleProfileNode() noexcept;
      void handleRouteNode() noexcept;

      [[nodiscard]] QSGGeometry::Point2D toPixel(float x, float y, float x_max, float y_max) const;
      [[nodiscard]] float toPixelX(float x, float x_max) const;
      [[nodiscard]] float toPixelY(float y, float y_max) const;

    protected:
      bool m_require_recolor;
      QSGGeometryNode* m_background_node;
      QSGGeometryNode* m_profile_node;
      QSGGeometryNode* m_route_node;

    private:
      SG::BasicPalette m_palette;
      bool m_intersecting;
      bool m_valid;
      Route m_route;
      RouteModel* m_model;
      QGeoCoordinate m_uav_position;

      unique_ptr<RandomDataProvider> m_random_provider;

      vector<ElevationPoint> m_profile;
      Bound m_bound;

      ShrinkMode m_shrink_mode;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::ChartItem*);