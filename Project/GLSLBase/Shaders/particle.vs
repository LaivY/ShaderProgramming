#version 450

in vec3			a_position;	// 위치
in vec3			a_velocity;	// 속도
in float		a_emitTime;	// 생성될 시각
in float		a_lifeTime;	// 수명

uniform float	u_time;		// 흐른시간
uniform vec3	u_accel;	// 가속도

float g_pi = 3.14f;

void main()
{
	float t = u_time - a_emitTime;
	if (t < 0)
	{
		gl_Position = vec4(-99.0f, -99.0f, -99.0f, 1.0f);
		return;
	}
	t = fract(t / a_lifeTime) * a_lifeTime;

	vec3 position;
	position.x = a_position.x + (a_velocity.x * t) + (0.5f * u_accel.x * pow(t, 2));
	position.y = a_position.y + 0.5f * sin(t * 2.0f * g_pi);
	position.z = 0.0f;
	gl_Position = vec4(position, 1.0f);
}