#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texc;

out vec3 vertexColor;
out vec2 texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	vertexColor = color;
	texcoord = texc;
	//...pode ter mais linhas de código aqui!
	gl_Position = projection * view * model * vec4(position, 1.0);
}