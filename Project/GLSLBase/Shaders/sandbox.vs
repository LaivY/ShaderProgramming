#version 450

in vec3 a_position;
in vec4 a_color;

out vec4 v_color;

void main()
{
	gl_Position = vec4(a_position, 1.0f);

	// rg값이 위치 좌표값이다.
	v_color = vec4(a_position.xy + 0.5f, a_position.z, 1.0f);
}