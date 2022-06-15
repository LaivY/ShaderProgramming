#version 450

layout(location=0) out vec4 FragColor;

uniform sampler2D u_sampler;

in vec2 v_uv;

void main()
{
	FragColor = texture(u_sampler, v_uv);
}