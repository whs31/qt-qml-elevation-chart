uniform lowp float qt_Opacity;
uniform lowp vec4 color;
varying highp vec2 texCoord;

void main()
{
    vec4 color_fade = vec4(1.0, 1.0, 1.0, 0.0) * 0.0;
    gl_FragColor = mix(color, color_fade, min(sin(radians(180.0) * texCoord.y), 0.3)) * qt_Opacity;
    //gl_FragColor = min(sin(radians(180.0) * texCoord.y) * 0.5, 0.3) * color * qt_Opacity;
}
