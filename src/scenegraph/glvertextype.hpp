#pragma once

struct State
{
    QColor color;
    int compare(const State* other) const {
        if (color.rgba() == other->color.rgba()) return 0;
        else if (color.rgba() < other->color.rgba()) return -1;
        else return 1;
    }
};

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
