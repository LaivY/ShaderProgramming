#version 450

in vec3 a_position;

out vec4 v_color;

uniform float u_time;

const float PI = 3.141592f;

void Flag()
{
	vec3 position = a_position;

	// 시작위치는 고정하고 그 뒤로 펄럭이는 것 처럼
	float base = -0.5f;
	float dist = distance(base, position.x);

	// 그리드 메쉬는 -0.5 ~ 0.5의 범위를 가진다.
	// 위치에 0.5를 더하면 0 ~ 1 사이의 값이고 사인의 한 주기는 2파이 이므로 2파이를 곱한다.
	// 또한 사인 결과값에 0.5를 곱해줘서 위치의 최대값이 0 ~ 1 사이가 되도록한다.
	// 또 이 결과에 dist 값을 곱하면 시작 위치 간의 거리 차이에 따라 저 크게 흔들린다.
	position.y += sin((position.x + 0.5f) * 2.0f * PI - u_time) * 0.5f * dist;
	gl_Position = vec4(position, 1.0f);

	v_color = vec4((sin((position.x + 0.5f) * 2.0f * PI - u_time) + 1.0f) / 2.0f);
}

void main()
{
	Flag();
}