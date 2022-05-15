#version 450

in vec3			a_position;	// 위치
in vec3			a_velocity;	// 속도
in float		a_emitTime;	// 생성될 시각
in float		a_lifeTime;	// 수명
in float		a_amp;		// 진폭
in float		a_freq;		// 주기
in float		a_value;	// 랜덤값
in vec4			a_color;	// 색깔

out vec4		v_color;
out vec2		v_uv;

uniform float	u_time;		// 흐른시간
uniform vec3	u_accel;	// 가속도

const float g_pi = 3.14f;
const mat3x3 g_rotate = mat3x3
(
	0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
);
const vec3 g_gravity = vec3(0.0f, -0.5f, 0.0f);

void main()
{
	float t = u_time - a_emitTime;
	if (t < 0)
	{
		gl_Position = vec4(-99.0f, -99.0f, -99.0f, 1.0f);
		v_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	// 소수부
	float fractional = fract(t / a_lifeTime);
	t = fractional * a_lifeTime;

	// 가속도
	vec3 accel = u_accel + g_gravity;

	// 시작 좌표가 원 위가 되도록 설정
	vec3 position = a_position;
	position.x += sin(a_value * 2.0f * g_pi);
	position.y += cos(a_value * 2.0f * g_pi);
	position.z += 0.0f;

	// 현재 위치에 속도를 더함
	position += (a_velocity * t) + (accel * pow(t, 2));

	// 속도에 수직인 벡터를 축으로 sin곡선을 만들도록 함
	vec3 rotVec = normalize(a_velocity * g_rotate);
	position += 0.1f * a_amp * rotVec * sin(a_freq * t * 2.0f * g_pi);

	gl_Position = vec4(position, 1.0f);

	v_color = a_color * (1.0f - fractional);
	v_uv = a_position.xy;
}