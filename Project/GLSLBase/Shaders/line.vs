#version 450

in vec3 a_position;

uniform float u_time;

const float PI = 3.141592f;

void main()
{
	float yPos = sin(a_position.x * PI + u_time);
	gl_Position = vec4(a_position.x, yPos, a_position.z, 1.0f);
}