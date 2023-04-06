#pragma once

#include <QtQuick/qsgsimplematerial.h>
#include <QFile>

struct State
{
    QColor color;
    int compare(const State* other) const {
        if (color.rgba() == other->color.rgba()) return 0;
        else if (color.rgba() < other->color.rgba()) return -1;
        else return 1;
    }
};

class GLPolygonShader : public QSGSimpleMaterialShader<State>
{
    QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(GLPolygonShader, State)

    int id_color;

    public:
        const char *vertexShader() const override {
            QFile file(":/polygon.vert");
            if(not file.open(QIODevice::ReadOnly | QIODevice::Text))
                return "Error reading vertex shader from file";
            return file.readAll().constData();
        }

        const char *fragmentShader() const override {
            QFile file(":/polygon.frag");
            if(not file.open(QIODevice::ReadOnly | QIODevice::Text))
                return "Error reading fragment shader from file";
            return file.readAll().constData();
        }

        QList<QByteArray> attributes() const override
        {
            return QList<QByteArray>() << "aVertex" << "aTexCoord";
        }

        void updateState(const State *state, const State *) override
        {
            program()->setUniformValue(id_color, state->color);
        }

        void resolveUniforms() override
        {
            id_color = program()->uniformLocation("color");
        }
};
