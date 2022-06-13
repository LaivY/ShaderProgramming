#version 450

uniform sampler2D u_sampler;

in vec2 v_uv;

out vec4 FragColor;

void main()
{
	vec2 uv = v_uv;
	uv.y = 1.0f - uv.y;
	FragColor = texture(u_sampler, uv);
}