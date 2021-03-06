#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_color;

uniform vec3 u_points[10];
uniform float u_lifeTimes[10];
uniform float u_time;

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
	const float circleCount = 10;
	float dist = distance(v_color.xy, vec2(0.5f, 0.5f));
	return vec4(sin(dist * circleCount * 4.0f * PI));
}

vec4 DrawCirclesAtPoints()
{
	vec4 result = vec4(0);
	for (int i = 0; i < 2; ++i)
	{
		float dist = distance(u_points[i].xy, v_color.xy);
		float temp = 10.0f * dist * 4.0f * PI - u_time;
		float value = sin(temp);

		if ((u_time - u_lifeTimes[i]) / 10.0f < dist && dist < u_time / 10.0f)
			result += vec4(value);
	}
	return result;
}

vec4 RadarCircle()
{
	const int thin = 1;				// 두께. 클 수록 얇아짐
	const float frequency = 5.0f;	// 주기. 클 수록 원 사이의 거리가 짧음
	const float speed = 10.0;		// 속도. 클 수록 원이 빠르게 움직임

	vec4 result = vec4(0);
	float dist = distance(vec2(0.5f, 0.0f), v_color.xy);
	float sinValue = sin(frequency * dist * PI - u_time * speed);
	sinValue = pow(sinValue, thin);
	sinValue = clamp(sinValue, 0.0f, 1.0f);
	result = vec4(sinValue);

	for (int i = 0; i < 10; ++i)
	{
		float d = distance(u_points[i].xy, v_color.xy);
		if (d < 0.1f)
			result += vec4(0.0f, (0.1f - d) * sinValue * 20.0f, 0.0f, 1.0f);
	}
	return result;
}

vec4 DrawSin()
{
	vec2 position = v_color.xy;

	float sinValue = 0.25f * sin(position.x * 2.0f * PI + u_time) + 0.5f;
	if (sinValue > position.y && sinValue - 0.01f < position.y)
		return vec4(1.0f);
	return vec4(0.0f);
}

void main()
{
	FragColor = DrawSin();
}