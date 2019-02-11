#version 330 core

// Interpolated values from the vertex shaders
in vec2 texCoord;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D colorTexture;

void main(){
	color = texture2D( colorTexture, texCoord );
}