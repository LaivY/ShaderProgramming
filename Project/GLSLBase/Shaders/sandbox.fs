#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_color;

const float PI = 3.141592f;

vec4 CrossPattern()
{
	const float frequency = 10.0f;
	vec4 result = vec4(0.0f);
	float x = sin(frequency * v_color.x * 2.0f * PI + PI * 1.5f);
	float y = sin(frequency * v_color.y * 2.0f * PI + PI * 1.5f);
	return vec4(max(x, y));
}

vec4 DrawCircle()
{
	vec4 result = vec4(1.0f);
	float dist = length(v_color.xy);
	if (dist > 0.05f)
	{
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	return result;
}

vec4 DrawCircleLine()
{
	float dist = length(v_color.xy);
	if (0.49f < dist && dist < 0.5f)
	{
		return vec4(1.0f);
	}
	return vec4(0.0f);
}

vec4 DrawCircles()
{
	const float radius = 0.05f;
	const float circleCount = 10;

	float dist = length(v_color.xy);
	return vec4(sin(dist * circleCount * 2.0f * PI));
}

void main()
{
	FragColor = DrawCircles();
}