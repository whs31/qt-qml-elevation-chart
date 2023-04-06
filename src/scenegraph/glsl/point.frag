uniform lowp float qt_Opacity;
uniform lowp vec4 color;
varying highp vec2 texCoord;

void main ()
{
    gl_FragColor = texCoord.x * texCoord.y * color * qt_Opacity;
}
