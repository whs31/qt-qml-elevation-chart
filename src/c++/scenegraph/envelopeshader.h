//
// Created by whs31 on 06.09.23.
//

#pragma once
#include <LPVL/Materials/FadingGradient>

namespace ElevationChart::GLSL
{
  class EnvelopeShader : public QSGSimpleMaterialShader<State>
  {
    public:
      static QSGMaterialShader* createShader();
      static QSGSimpleMaterialComparableMaterial<State>* createMaterial();

      const char* vertexShader() const override;
      const char* fragmentShader() const override;
      QList<QByteArray> attributes() const override;
      void updateState(const State* state, const State*) override;
      void resolveUniforms() override;

    private:
      int id_color;
  };
} // ElevationChart::GLSL
