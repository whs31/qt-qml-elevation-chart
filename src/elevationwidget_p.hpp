#pragma once

#include "charts/elevationwidget.hpp"

namespace Elevation {
    class Elevation;
    class ElevationTools;
}

namespace Charts
{
    /// @private
    class ElevationWidgetPrivate : public QObject
    {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ElevationWidget)

        Elevation::Elevation* heightmapParser;
        Elevation::ElevationTools* routeParser;

        public:
            explicit ElevationWidgetPrivate(ElevationWidget* parent);
            virtual ~ElevationWidgetPrivate() = default;

        protected:
            ElevationWidget* q_ptr;
    };
} ///namespace Charts;

inline void initialize_qrc_file_within_namespace_1(const char* comment)
{
    Q_INIT_RESOURCE(charts);
}
