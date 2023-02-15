#ifndef MACRO_HPP
#define MACRO_HPP

// universal

#define QPR(type, name, setterName) \
        type m_##name;    \
        QPR_GET(type, name) \
        QPR_SET(type, name, setterName)

#define QPR_M(type, name, setterName, member) \
        QPR_GET_M(type, name, member)  \
        QPR_SET_M(type, name, setterName, member)

#define QPR_M_UPDATE(type, name, setterName, member) \
        QPR_GET_M(type, name, member)  \
        QPR_SET_M_UPDATE(type, name, setterName, member)

// 4headers
#define QPR_GET(pr_type, pr_name) \
        pr_type pr_name () const { return m_##pr_name; }

#define QPR_SET(pr_type, pr_name, pr_setterName) \
        void pr_setterName ( pr_type val )  \
        {   \
            if (m_##pr_name == val) return; \
            m_##pr_name = val; \
            emit pr_name##Changed(); \
        }

#define QPR_GET_M(pr_type, pr_name, member) \
        pr_type pr_name () const { return member; }

#define QPR_SET_M(pr_type, pr_name, pr_setterName, member) \
        void pr_setterName ( pr_type val )  \
        {   \
            if (member == val) return; \
            member = val; \
            emit pr_name##Changed(); \
        }

#define QPR_SET_NOCHECK(pr_type, pr_name, pr_setterName) \
        void pr_setterName ( pr_type val )  \
        {   \
            m_##pr_name = val; \
            emit pr_name##Changed(); \
        }

#define QPR_SET_M_NOCHECK(pr_type, pr_name, pr_setterName, member) \
        void pr_setterName ( pr_type val )  \
        {   \
            member = val; \
            emit pr_name##Changed(); \
        }

// все, что ниже, можно
// использовать только в рамках этого сабмодуля,
// остальное универсально

#define QPR_SET_M_UPDATE(pr_type, pr_name, pr_setterName, member) \
        void pr_setterName ( pr_type val )  \
        {   \
            if (member == val) return; \
            member = val; \
            emit pr_name##Changed(); \
            if(!m_geopath.isEmpty() && axes.x.pixelsize > 0) update(false); \
        }

#define QPR_SET_NOCHECK_U(pr_type, pr_name, pr_setterName) \
        void pr_setterName ( pr_type val )  \
        {   \
            m_##pr_name = val; \
            emit pr_name##Changed(); \
        if(axes.x.pixelsize > 0) update(true); \
        }

#endif // MACRO_HPP
