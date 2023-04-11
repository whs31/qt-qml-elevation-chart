uniform lowp float qt_Opacity;
uniform lowp vec4 color;
varying highp vec2 texCoord;
void main ()
{
    gl_FragColor = min(1.5 - texCoord.y, 1.0) * color * qt_Opacity;
}
