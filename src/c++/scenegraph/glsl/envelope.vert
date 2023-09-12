attribute highp vec4 aVertex;
attribute highp vec2 aTexCoord;
uniform highp mat4 qt_Matrix;
varying highp vec2 texCoord;

void main() {
    gl_Position = qt_Matrix * aVertex;
    texCoord = aTexCoord;
}

