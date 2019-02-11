#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexTexCoord;

out vec2 texCoord;


void main(){


	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  vec4(position, 1.0);
	texCoord = vertexTexCoord;

}

