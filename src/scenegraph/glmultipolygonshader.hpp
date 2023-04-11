#pragma once

#include "glvertextype.hpp"

#include <QtQuick/qsgsimplematerial.h>
#include <QFile>

class GLMultipolygonShader : public QSGSimpleMaterialShader<State>
{
    QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(GLMultipolygonShader, State)

    int id_color;

    public:
        const char *vertexShader() const override {
            QFile file(":/multipolygon.vert");
            if(not file.open(QIODevice::ReadOnly | QIODevice::Text))
                return "Error reading vertex shader from file";
            return file.readAll().constData();
        }

        const char *fragmentShader() const override {
            QFile file(":/multipolygon.frag");
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
