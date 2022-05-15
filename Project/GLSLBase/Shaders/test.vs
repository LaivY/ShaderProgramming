#version 450

in vec3 a_Position;

uniform float u_time;

void main()
{
	gl_Position = vec4(a_Position * u_time, 1);
}