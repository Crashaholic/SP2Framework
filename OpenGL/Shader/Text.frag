#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;

// Ouput data
out vec4 color;

uniform sampler2D atlas;
uniform vec3 textColor;

uniform float fontSize;

float width;
float edge;

void main()
{
	width = fontSize / (fontSize * 2) + fontSize * 0.001;
	edge = (fontSize * 0.01) / (2 * (fontSize / (5 / fontSize)));

	float dist = 1.0 - texture(atlas, texCoord).a;
	float alpha = 1.0 - smoothstep(width, width + edge, dist);

	color = vec4(textColor, alpha);
}