//
// Created by whs31 on 03.08.23.
//

#pragma once

#include <memory>
#include <QtCore/QPointF>
#include <QtQuick/QQuickItem>
#include <QtQuick/QSGGeometry>
#include <SG/BasicPalette>
#include "types/route.h"
#include "types/bounds.h"
#include "types/metrics.h"
#include "provider/randomdataprovider.h"
#include "internal/routemodel.h"

using std::unique_ptr;

class QSGGeometryNode;

namespace ElevationChart
{
  class ElevationChartItem : public QQuickItem
  {
    Q_OBJECT
    Q_PROPERTY(SG::BasicPalette palette READ palette WRITE setPalette NOTIFY paletteChanged FINAL)
    Q_PROPERTY(ElevationChart::Bounds bounds READ bounds WRITE setBounds NOTIFY boundsChanged)
    Q_PROPERTY(ElevationChart::Route route READ route WRITE setRoute NOTIFY routeChanged FINAL)
    Q_PROPERTY(ElevationChart::RouteModel* model READ model CONSTANT)
    Q_PROPERTY(QGeoCoordinate uavPosition READ uavPosition WRITE setUavPosition NOTIFY uavPositionChanged FINAL)
    Q_PROPERTY(ElevationChart::Metrics metrics READ metrics WRITE setMetrics NOTIFY metricsChanged FINAL)
    // envelope gadget

    Q_PROPERTY(bool intersecting READ intersecting WRITE setIntersecting NOTIFY intersectingChanged FINAL)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged FINAL)
    Q_PROPERTY(bool matchingMetrics READ matchingMetrics WRITE setMatchingMetrics NOTIFY matchingMetricsChanged FINAL)
    // is busy

    Q_PROPERTY(int shrinkMode READ shrinkMode WRITE setShrinkMode NOTIFY shrinkModeChanged FINAL)

    constexpr static const float ROUTE_LINE_WIDTH = 5.0f;         ///< Ширина линии пути для отрисовки (px).
    constexpr static const float METRICS_LINE_WIDTH = 3.0f;       ///< Ширина линии пути, скорректированного по ЛТХ для отрисовки (px).
    constexpr static const float METRICS_ROUNDING_WIDTH = 8.0f;   ///< Размер точек для сглаживания ломаной на пути ЛТХ (px).

    public:
      /// \brief Перечисление поведения оси Y.
      enum ShrinkMode
      {
        ShrinkToProfileHeight,    ///< Ось Y будет ограничена сверху высотой профиля высот, либо высотой пути, если таковая больше.
        ShrinkToRouteHeight       ///< Ось Y будет ограничена сверху высотой пути. Если профиль рельефа выше, он будет обрезан.
      };
      Q_ENUM(ShrinkMode)

      explicit ElevationChartItem(QQuickItem* parent = nullptr);

      [[nodiscard]] SG::BasicPalette palette() const;   void setPalette(SG::BasicPalette);
      [[nodiscard]] Bounds bounds() const;              void setBounds(Bounds);
      [[nodiscard]] Route route() const;                void setRoute(const Route&);
      [[nodiscard]] RouteModel* model() const;
      [[nodiscard]] QGeoCoordinate uavPosition() const; void setUavPosition(const QGeoCoordinate&);
      [[nodiscard]] Metrics metrics() const;            void setMetrics(const Metrics&);

      [[nodiscard]] bool intersecting() const;          void setIntersecting(bool);
      [[nodiscard]] bool valid() const;                 void setValid(bool);
      [[nodiscard]] bool matchingMetrics() const;       void setMatchingMetrics(bool);

      [[nodiscard]] int shrinkMode() const;             void setShrinkMode(int);

      Q_INVOKABLE void applyMetricsCorrection() noexcept;
      //Q_INVOKABLE void estimateEnvelope() noexcept;
      //Q_INVOKABLE void applyEnvelopeCorrection() noexcept;

    signals:
      void paletteChanged();
      void boundsChanged();
      void intersectingChanged();
      void validChanged();
      void matchingMetricsChanged();
      void routeChanged();
      void uavPositionChanged();
      void metricsChanged();
      void shrinkModeChanged();

    protected:
      QSGNode* updatePaintNode(QSGNode* old_node, UpdatePaintNodeData*) override;
      void requireRecolor();
      void fulfillRecolor();

      Q_SLOT void updateProfile() noexcept;
      Q_SLOT void updateBounds() noexcept;
      Q_SLOT void updateMetrics() noexcept;

      void handleBackgroundNode() noexcept;
      void handleProfileNode() noexcept;
      void handleRouteNode() noexcept;
      void handleMetricsNode() noexcept;

      [[nodiscard]] QSGGeometry::Point2D toPixel(float x, float y, float x_max, float y_max) const;
      [[nodiscard]] float toPixelX(float x, float x_max) const;
      [[nodiscard]] float toPixelY(float y, float y_max) const;

    protected:
      QSGGeometryNode* m_background_node;     ///< Нода фона.
      QSGGeometryNode* m_profile_node;        ///< Нода профиля рельефа.
      QSGGeometryNode* m_route_node;          ///< Нода маршрута.
      QSGGeometryNode* m_metrics_node;        ///< Нода маршрута, скорректированного по ЛТХ.
      QSGGeometryNode* m_metrics_point_node;  ///< Нода для точек маршрута, скорректированного по ЛТХ.
      QSGGeometryNode* m_envelope_node;       ///< Нода огибающей.
      QSGGeometryNode* m_corridor_node;       ///< Нода коридора для огибающей.

    private:
      bool m_require_recolor;
      SG::BasicPalette m_palette;
      Bounds m_bounds;
      bool m_intersecting;
      bool m_valid;
      bool m_matching_metrics;
      Route m_route;
      RouteModel* m_model;
      QGeoCoordinate m_uav_position;
      Metrics m_metrics;
      unique_ptr<RandomDataProvider> m_random_provider;
      vector<ElevationPoint> m_profile;
      ShrinkMode m_shrink_mode;
      QGeoPath m_metrics_path;
      QGeoPath m_envelope_path;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::ElevationChartItem*);