#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;


// Output data ; will be interpolated for each fragment.
out vec2 texCoord;

uniform mat4 transformationMatrix;
uniform int dontFlip;
uniform float rotation;
uniform int shouldRotate;

void main(){


	
	gl_Position =  transformationMatrix * vec4(vertexPosition_modelspace.x, vertexPosition_modelspace.y, 0.0, 1);

	texCoord = vec2( (vertexPosition_modelspace.x + 1.0) / 2.0, dontFlip * (1 - (vertexPosition_modelspace.y + 1.0)/2.0));
//	if(shouldRotate == 1)
//		texCoord = t * mat2(cos(rotation), sin(rotation), -sin(rotation), cos(rotation));
//	else
//		texCoord = t;

}

