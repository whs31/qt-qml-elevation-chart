uniform lowp float qt_Opacity;
uniform lowp vec4 color;
varying highp vec2 texCoord;
void main ()
{
    gl_FragColor = 1 * min(1.3 - texCoord.y, 1.0) * color;
}
