//
// Created by whs31 on 06.09.23.
//

#include "envelopeshader.h"
#include <QtCore/QFile>

namespace ElevationChart::GLSL
{
  QSGMaterialShader* EnvelopeShader::createShader() { return new EnvelopeShader; }
  QSGSimpleMaterialComparableMaterial<State>* EnvelopeShader::createMaterial() {
    return new QSGSimpleMaterialComparableMaterial<State>(EnvelopeShader::createShader);
  }

  const char* EnvelopeShader::vertexShader() const
  {
    QFile file(":/elevation-chart/envelope.vert");
    if(not file.open(QIODevice::ReadOnly | QIODevice::Text))
      return "Error reading vertex shader from file";
    return file.readAll().constData();
  }

  const char* EnvelopeShader::fragmentShader() const
  {
    QFile file(":/elevation-chart/envelope.frag");
    if(not file.open(QIODevice::ReadOnly | QIODevice::Text))
      return "Error reading fragment shader from file";
    return file.readAll().constData();
  }

  QList<QByteArray> EnvelopeShader::attributes() const { return QList<QByteArray>() << "aVertex" << "aTexCoord"; }
  void EnvelopeShader::updateState(const State* state, const State*)
  {
    program()->setUniformValue(id_color, state->color);
  }
  void EnvelopeShader::resolveUniforms()
  {
    id_color = program()->uniformLocation("color");
    time = program()->uniformLocation("time");
  }
} // ElevationChart::GLSL