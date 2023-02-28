#ifndef MACRO_HPP
#define MACRO_HPP

// one macro to rule them all

#define PROPERTY(type, name) \
        private: \
            type m_##name; \
            type name () const { return m_##name; } \
            void name##Set ( type val )  \
            {   \
                if (m_##name == val) return; \
                m_##name = val; \
                emit name##Changed(); \
            }   \
        Q_PROPERTY(type name READ name WRITE name##Set NOTIFY name##Changed) \

#define EMIT Q_SIGNALS: void
#define END (); private:

// more precise types:
#define PROPERTY_NOCOMPARE(type, name) \
        private: \
            type m_##name; \
            type name () const { return m_##name; } \
            void name##Set ( type val )  \
            {   \
                m_##name = val; \
                emit name##Changed(); \
            }   \
        Q_PROPERTY(type name READ name WRITE name##Set NOTIFY name##Changed) \

#define PROPERTY_NOCOMPARE_SET_ARGS(type, name) \
        Q_PROPERTY(type name READ name WRITE name##Set NOTIFY name##Changed) \
        private: \
            type m_##name; \
            type name () const { return m_##name; } \
            void name##Set ( type val )  \
            {   \
                m_##name = val; \
                emit name##Changed(); \

#define PROPERTY_SET_ARGS(type, name) \
        Q_PROPERTY(type name READ name WRITE name##Set NOTIFY name##Changed) \
        private: \
            type m_##name; \
            type name () const { return m_##name; } \
            void name##Set ( type val )  \
            {   \
                if (m_##name == val) return; \
                m_##name = val; \
                emit name##Changed(); \

#endif // MACRO_HPP
