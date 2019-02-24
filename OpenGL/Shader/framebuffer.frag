#version 330 core

out vec4 FragColor;

in vec2 Uv;

uniform sampler2D screenTexture;


float lerp(float a, float b, float f)
{
    return a + f * (b - a);
} 

void main()
{
	vec4 res;
	vec3 col = texture(screenTexture, Uv).rgb;
	vec2 uv = Uv;
	uv *= 1.0 - Uv.yx;
	float vig = uv.x * uv.y * 15.0;
	vig = pow(vig, 0.25);
	res = vec4(texture(screenTexture, Uv).rgb * vig, 1.0);
//	res = vec4(texture(screenTexture, Uv));
	FragColor = res;
}