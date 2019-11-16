#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
uniform vec2 screenSize;

void main()
{
	gl_Position = vec4(2 * position/screenSize - 1.0f, 0.0, 1.0f);
}


#shader fragment
#version 330 core

layout (location = 0) out vec4 color;
uniform vec2 center;
uniform vec4 stationColor;
uniform float stationRadius;
uniform vec2 screenSize;

void main()
{
	vec2 st = (gl_FragCoord.xy);
	vec2 distVec = st - center;
	float dist = distance(st, center);
	if (dist <= stationRadius) color = stationColor/*vec4(stationColor.xyz, 1 - (dist / stationRadius))*/;
	else discard;
}
