#pragma once

namespace ChartsOpenGL {
    struct Vertex
    {
        Vertex(float _x, float _y) : x(_x), y(_y) {}
        float x;
        float y;
    };

    struct VertexT
    {
        VertexT(float _x, float _y, float _u = 1, float _v = 1) : x(_x), y(_y), u(_u), v(_v) {}
        float x;
        float y;
        float u;
        float v;
    };
}
