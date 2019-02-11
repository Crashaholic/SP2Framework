#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;

// Ouput data
out vec4 color;

uniform sampler2D atlas;
uniform vec3 textColor;

void main(){
	color = vec4(textColor, texture(atlas, texCoord).a);
}