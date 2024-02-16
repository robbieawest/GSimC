#version 440 core

layout(location = 0) in vec4 position;
uniform mat4 projectionAllMat;

void main(){
	gl_Position = projectionAllMat * position;
}
