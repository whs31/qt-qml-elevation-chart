//
// Created by whs31 on 03.08.23.
//

#pragma once

#include <memory>
#include <map>
#include <QtCore/QPointF>
#include <QtQuick/QSGGeometry>
#include <LPVL/ScenegraphObject>
#include "types/palette.h"
#include "types/route.h"
#include "types/bounds.h"
#include "types/metrics.h"
#include "provider/randomdataprovider.h"
#include "provider/demdataprovider.h"
#include "internal/routemodel.h"
#include "internal/notificationmodel.h"
#include "internal/axismodel.h"
#include "research/researcher.h"

using std::unique_ptr;
using std::map;

class QSGGeometryNode;

namespace ElevationChart
{
  class Analyzer;
  class ElevationChartItem : public LPVL::ScenegraphObject
  {
    Q_OBJECT
    Q_PROPERTY(ElevationChart::Palette palette READ palette WRITE setPalette NOTIFY paletteChanged FINAL)
    Q_PROPERTY(ElevationChart::Bounds bounds READ bounds WRITE setBounds NOTIFY boundsChanged)
    Q_PROPERTY(ElevationChart::Route route READ route WRITE setRoute NOTIFY routeChanged FINAL)
    Q_PROPERTY(ElevationChart::RouteModel* model READ model CONSTANT FINAL)
    Q_PROPERTY(ElevationChart::Researcher* researcher READ researcher CONSTANT FINAL)
    Q_PROPERTY(ElevationChart::NotificationModel* notifications READ notifications CONSTANT FINAL)
    Q_PROPERTY(QGeoCoordinate uavPosition READ uavPosition WRITE setUavPosition NOTIFY uavPositionChanged FINAL)
    Q_PROPERTY(ElevationChart::Metrics metrics READ metrics WRITE setMetrics NOTIFY metricsChanged FINAL)
    Q_PROPERTY(ElevationChart::Envelope envelope READ envelope WRITE setEnvelope NOTIFY envelopeChanged FINAL)
    Q_PROPERTY(ElevationChart::AxisModel* yRelativeModel READ yRelativeModel CONSTANT FINAL)
    Q_PROPERTY(ElevationChart::AxisModel* yAbsoluteModel READ yAbsoluteModel CONSTANT FINAL)

    Q_PROPERTY(bool missingTiles READ missingTiles WRITE setMissingTiles NOTIFY missingTilesChanged FINAL)
    Q_PROPERTY(bool intersecting READ intersecting WRITE setIntersecting NOTIFY intersectingChanged FINAL)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged FINAL)
    Q_PROPERTY(bool routeValid READ routeValid NOTIFY routeValidChanged STORED false FINAL)
    Q_PROPERTY(bool matchingMetrics READ matchingMetrics WRITE setMatchingMetrics NOTIFY matchingMetricsChanged FINAL)
    Q_PROPERTY(bool allowEnvelopeCorrection READ allowEnvelopeCorrection NOTIFY allowEnvelopeCorrectionChanged STORED false FINAL)

    Q_PROPERTY(int shrinkMode READ shrinkMode WRITE setShrinkMode NOTIFY shrinkModeChanged FINAL)
    Q_PROPERTY(int providerType READ providerType CONSTANT FINAL)

    Q_PROPERTY(QPointF uavVisualPosition READ uavVisualPosition WRITE setUavVisualPosition NOTIFY uavVisualPositionChanged FINAL)
    Q_PROPERTY(float uavVisualAngle READ uavVisualAngle WRITE setUavVisualAngle NOTIFY uavVisualAngleChanged FINAL)

    constexpr static const float ROUTE_LINE_WIDTH = 5.0f;         ///< Ширина линии пути для отрисовки (px).
    constexpr static const float METRICS_LINE_WIDTH = 3.0f;       ///< Ширина линии пути, скорректированного по ЛТХ для отрисовки (px).
    constexpr static const float METRICS_ROUNDING_WIDTH = 8.0f;   ///< Размер точек для сглаживания ломаной на пути ЛТХ (px).
    constexpr static uint8_t ENVELOPE_NODE_OPACITY = 100;
    constexpr static uint8_t CORRIDOR_NODE_OPACITY = 50;

    friend class Analyzer;

    enum NodeTypes
    {
      BackgroundNode,         ///< Нода фона.
      ProfileNode,            ///< Нода профиля рельефа.
      IntersectionsNode,      ///< Нода пересечений с рельефом.
      MetricsNode,            ///< Нода маршрута, скорректированного по ЛТХ.
      MetricsPointNode,       ///< Нода для точек маршрута, скорректированного по ЛТХ.
      EnvelopeNode,           ///< Нода огибающей.
      EnvelopePointNode,      ///< Нода для точек огибающей.
      CorridorNode,           ///< Нода коридора для огибающей.
      RouteNode,              ///< Нода маршрута.
      IntersectionsRouteNode  ///< Нода индикации пересечений с рельефом на маршруте.
    };

    public:
      /// \brief Перечисление поведения оси Y.
      enum ShrinkMode
      {
        ShrinkToProfileHeight,    ///< Ось Y будет ограничена сверху высотой профиля высот, либо высотой пути, если таковая больше.
        ShrinkToRouteHeight       ///< Ось Y будет ограничена сверху высотой пути. Если профиль рельефа выше, он будет обрезан.
      };
      Q_ENUM(ShrinkMode)

      /// \brief Перечисление типов провайдера высотных данных.
      enum ProviderType
      {
        RandomProvider,           ///< Тестовый провайдер (см. RandomDataProvider).
        DEMProvider               ///< Провайдер библиотеки DEM (см. DEMDataProvider).
      };

      explicit ElevationChartItem(QQuickItem* parent = nullptr);

      [[nodiscard]] Researcher* researcher() const;

      [[nodiscard]] Palette palette() const;            void setPalette(Palette);
      [[nodiscard]] Bounds bounds() const;              void setBounds(Bounds);
      [[nodiscard]] Route route() const;                void setRoute(const Route&);
      [[nodiscard]] RouteModel* model() const;
      [[nodiscard]] NotificationModel* notifications() const;
      [[nodiscard]] QGeoCoordinate uavPosition() const; void setUavPosition(const QGeoCoordinate&);
      [[nodiscard]] Metrics metrics() const;            void setMetrics(const Metrics&);
      [[nodiscard]] Envelope envelope() const;          void setEnvelope(const Envelope&);
      [[nodiscard]] AxisModel* yRelativeModel() const;
      [[nodiscard]] AxisModel* yAbsoluteModel() const;

      [[nodiscard]] bool missingTiles() const;          void setMissingTiles(bool);
      [[nodiscard]] bool intersecting() const;          void setIntersecting(bool);
      [[nodiscard]] bool valid() const;                 void setValid(bool);
      [[nodiscard]] bool routeValid() const;
      [[nodiscard]] bool matchingMetrics() const;       void setMatchingMetrics(bool);
      [[nodiscard]] bool allowEnvelopeCorrection() const;

      [[nodiscard]] int shrinkMode() const;             void setShrinkMode(int);
      [[nodiscard]] int providerType() const;
      [[nodiscard]] QPointF uavVisualPosition() const;  void setUavVisualPosition(QPointF);
      [[nodiscard]] float uavVisualAngle() const;       void setUavVisualAngle(float);

      invokable [[maybe_unused]] void applyMetricsCorrection() noexcept;
      invokable [[maybe_unused]] void estimateEnvelope() const noexcept;
      invokable [[maybe_unused]] void applyEnvelopeCorrection() noexcept;
      invokable [[maybe_unused]] void setUavRelativeHeight() noexcept;

      invokable static QColor mixAlpha(QColor, float a) noexcept;

    signals:
      void paletteChanged();
      void boundsChanged();
      void missingTilesChanged();
      void intersectingChanged();
      void validChanged();
      void routeValidChanged();
      void matchingMetricsChanged();
      void routeChanged();
      void uavPositionChanged();
      void metricsChanged();
      void envelopeChanged();
      void shrinkModeChanged();
      void allowEnvelopeCorrectionChanged();
      void uavVisualPositionChanged();
      void uavVisualAngleChanged();

      void updateProfileFinished(const vector<ElevationPoint>& profile);

      void envelopeAccepted(ElevationChart::Route route);
      void metricsAccepted(ElevationChart::Route route);
      void pointAccepted(int index, ElevationChart::RoutePoint point);


    protected:
      void setupChildNodes(QSGNode* node) final;
      void setupNodeColors(QSGNode* node) final;
      void drawCall(QSGNode* node) final;

      Q_SLOT void updateProfile() noexcept;
      Q_SLOT void receiveProfile(const vector<ElevationPoint>& profile) noexcept;
      Q_SLOT void updateBounds() noexcept;
      Q_SLOT void updateAxes() noexcept;
      Q_SLOT void updateIndividualPoint(int index, float alt) noexcept;
      Q_SLOT void updateIntersections(const vector<ElevationPoint>&) noexcept;
      Q_SLOT void updateEnvelope(const Researcher::EnvelopeResult&) noexcept;
      Q_SLOT void updateMetrics() noexcept;
      void updateUavVisualPosition() noexcept;

      void handleBackgroundNode() noexcept;
      void handleProfileNode() noexcept;
      void handleMetricsNode() noexcept;
      void handleIntersectionsNode() noexcept;

      void handle(QSGGeometry* geometry, const vector<ElevationPoint>& vec, bool abort_condition = false);
      void handle2nodes(QSGGeometry* geometry1, QSGGeometry* geometry2, const vector<ElevationPoint>& vec, bool abort_condition = false);

      [[nodiscard]] QSGGeometry::Point2D toPixel(float x, float y, Bounds b = Bounds()) const;
      [[nodiscard]] QSGGeometry::Point2D toPixel(ElevationPoint p, Bounds b = Bounds()) const;
      [[nodiscard]] float toPixelX(float x, float x_max) const;
      [[nodiscard]] float toPixelY(float y, float y_max) const;

      [[nodiscard]] map<NodeTypes, QSGGeometryNode*>& tree();
      [[nodiscard]] IElevationDataProvider* provider();

    protected:
      map<NodeTypes, QSGGeometryNode*> m_tree;

    private:
      Palette m_palette;
      Researcher* m_researcher;
      Bounds m_bounds;
      bool m_missing_tiles;
      bool m_intersecting;
      bool m_valid;
      bool m_matching_metrics;
      Route m_route;
      RouteModel* m_model;
      NotificationModel* m_notifications;
      QGeoCoordinate m_uav_position;
      Metrics m_metrics;
      Envelope m_envelope;
      AxisModel* m_y_relative_model;
      AxisModel* m_y_absolute_model;
      unique_ptr<IElevationDataProvider> m_provider;
      vector<ElevationPoint> m_profile;
      vector<ElevationPoint> m_intersections;
      vector<ElevationPoint> m_envelopePathVec;
      vector<ElevationPoint> m_envelopeCorridorVec;
      Route m_envelope_route;
      ShrinkMode m_shrink_mode;
      ProviderType m_provider_type;
      QGeoPath m_metrics_path;
      QGeoPath m_envelope_path;
      QPointF m_uav_visual_pos;
      float m_uav_visual_angle;
      float m_stored_uav_height;
  };
} // ElevationChart

Q_DECLARE_METATYPE(ElevationChart::ElevationChartItem*);
