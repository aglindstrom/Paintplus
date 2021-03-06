#version 400

layout(location = 0)in vec3 inputPosition;
layout(location = 1)in vec2 inputTexCoord;

out vec2 texCoord;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	gl_Position = worldMatrix * vec4(inputPosition, 1.0f);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	texCoord = inputTexCoord;
}