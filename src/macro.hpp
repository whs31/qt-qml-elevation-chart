#ifndef MACRO_HPP
#define MACRO_HPP

#define P_QREAL(getter, member, setter, signal) \
        qreal ElevationChart::getter () const { return member; } \
        void ElevationChart::setter (qreal value) { \
        if (qFuzzyCompare(member, value)) { \
        return; }\
        member = value; emit signal(); }
#define P_QREAL_NOCHECK(getter, member, setter, signal) \
        qreal ElevationChart::getter () const { return member; } \
        void ElevationChart::setter (qreal value) { \
        member = value; emit signal(); }
#define P_QREAL_UPDATE(getter, member, setter, signal) \
        qreal ElevationChart::getter () const { return member; } \
        void ElevationChart::setter (qreal value) { \
        if (qFuzzyCompare(member, value)) { \
        return; }\
        member = value; emit signal(); if(!m_geopath.isEmpty() && axes.x.pixelsize > 0) update(false); }
#define P_INT(getter, member, setter, signal) \
        int ElevationChart::getter () const { return member; } \
        void ElevationChart::setter (int value) { \
        if (member == value) { \
        return; }\
        member = value; emit signal(); }
#define P_BOOLEAN(getter, member, setter, signal) \
        bool ElevationChart::getter () const { return member; } \
        void ElevationChart::setter (bool value) { \
        if (member == value) { \
        return; }\
        member = value; emit signal(); }

#endif // MACRO_HPP
