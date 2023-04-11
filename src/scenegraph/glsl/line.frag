uniform lowp float qt_Opacity;
uniform lowp vec4 color;
varying highp vec2 texCoord;

void main ()
{
//	vec2 dir = texCoord.xy * vec2(5.0, 5.0) / 2.0;
//	float dist = length(dir);
//	if(fract(dist / (5.0 + 5.0)) > 5.0 / (5.0 + 5.0))
//		discard;
    gl_FragColor = min(1.5 - texCoord.y, 1.0) * color * qt_Opacity;
}
