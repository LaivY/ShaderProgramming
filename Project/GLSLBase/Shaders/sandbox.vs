#version 450

in vec3 a_position;
in vec4 a_color;

out vec4 v_color;

void main()
{
	gl_Position = vec4(a_position, 1.0f);

	// rg값이 위치 좌표값이다.
	v_color.x = clamp(a_position.x + 1.0f, 0.0f, 1.0f);
	v_color.y = clamp(a_position.y + 1.0f, 0.0f, 1.0f);
	v_color.zw = vec2(a_position.z, 1.0f);
}