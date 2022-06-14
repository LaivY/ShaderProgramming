#version 450

in vec3 a_position;

out vec4 v_color;

uniform float u_time;

const float PI = 3.141592f;

void Flag()
{
	vec3 position = a_position;

	// ������ġ�� �����ϰ� �� �ڷ� �޷��̴� �� ó��
	float base = -0.5f;
	float dist = distance(base, position.x);

	// �׸��� �޽��� -0.5 ~ 0.5�� ������ ������.
	// ��ġ�� 0.5�� ���ϸ� 0 ~ 1 ������ ���̰� ������ �� �ֱ�� 2���� �̹Ƿ� 2���̸� ���Ѵ�.
	// ���� ���� ������� 0.5�� �����༭ ��ġ�� �ִ밪�� 0 ~ 1 ���̰� �ǵ����Ѵ�.
	// �� �� ����� dist ���� ���ϸ� ���� ��ġ ���� �Ÿ� ���̿� ���� �� ũ�� ��鸰��.
	position.y += sin((position.x + 0.5f) * 2.0f * PI - u_time) * 0.5f * dist;
	gl_Position = vec4(position, 1.0f);

	v_color = vec4((sin((position.x + 0.5f) * 2.0f * PI - u_time) + 1.0f) / 2.0f);
}

void main()
{
	Flag();
}