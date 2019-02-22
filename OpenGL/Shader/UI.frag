#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;

// Ouput data
out vec4 color;

uniform vec3 elementColor;
uniform float alpha;

uniform int useSolidColor;

// Values that stay constant for the whole mesh.
uniform sampler2D colorTexture;

void main()
{
	vec4 res = vec4(elementColor, alpha);
//	if (useSolidColor == true)
//	{
//		color = res;
//	}
//	else
//	{
//		color = texture2D( colorTexture, texCoord );
//	}
	color = (res * useSolidColor) + ((1 - useSolidColor) * texture2D(colorTexture, texCoord));
}