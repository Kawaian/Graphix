#version 330 core

uniform mat4 MVP;

in vec3 position;

out vec3 texCoord0;

void main(){
	vec4 MVP_pos = MVP * vec4(position, 1.0);	
	gl_Position = MVP_pos.xyww;
	texCoord0 = position;
}
