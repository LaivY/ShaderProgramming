#version 450

uniform sampler2D u_sampler;

in vec2 v_uv;

out vec4 FragColor;

vec4 Flip()
{
	vec2 uv = v_uv;
	uv.y = 1.0f - uv.y;
	return texture(u_sampler, uv);
}

vec4 Mirror()
{
	vec2 uv = v_uv;
	uv.y = abs(uv.y * 2.0f - 1.0f);
	return texture(u_sampler, uv);
}

vec4 Rotate()
{
	vec2 uv = vec2(0.0f);
	uv.x = v_uv.x / 3.0f + floor(v_uv.y * 3.0f) / 3.0f;
	uv.y = fract(v_uv.y * 3.0f);
	return texture(u_sampler, uv);
}

vec4 FlipRotate()
{
	vec2 uv = vec2(0.0f);
	uv.x = v_uv.x / 3.0f + floor((1.0f - v_uv.y) * 3.0f) / 3.0f;
	uv.y = fract(v_uv.y * 3.0f);
	return texture(u_sampler, uv);
}

vec4 BlockX()
{
	float count = 2.0f;

	vec2 uv = vec2(0.0f);
	uv.x = fract(v_uv.x * count);
	uv.y = fract(v_uv.y * count) + floor(v_uv.x * count) * 0.5f;
	return texture(u_sampler, uv);
}

vec4 BlockY()
{
	float count = 5.0f;

	vec2 uv = vec2(0.0f);
	uv.x = fract(v_uv.x * count) + floor(v_uv.y * count) * 0.5f;
	uv.y = fract(v_uv.y * count);
	return texture(u_sampler, uv);
}

void main()
{
	FragColor = BlockX();
}