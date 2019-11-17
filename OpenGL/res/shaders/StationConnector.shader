#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
//uniform vec2 screenSize;
out vec2 pos;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	pos = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
//uniform vec2 screenSize;
uniform vec2 center;
uniform float len;
in vec2 pos;

void main()
{
	vec4 color1 = vec4(0.2706, 0.6353, 0.2784, 1.0);
	vec4 color2 = vec4(0.1568, 0.2353, 0.5255, 1.0);
	float dist = distance(gl_FragCoord.xy, center);
	//color = vec4(0.0, (pos + 1.0) / 2.0, 1.0f);
	color = mix(color1, color2, dist/len);
}
