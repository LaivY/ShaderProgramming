#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_color;

void main()
{
	float r = length(v_color.xy - vec2(0.5f, 0.5f));
	if (0.48f < r && r < 0.5f)
	{
		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		return;
	}
	FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}