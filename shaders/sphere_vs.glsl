#version 330

uniform vec3 position;

void main()
{
	gl_Position = vec4(position.xyz, 1);
	//gl_Position = vec4(2, 2, 0, 1);
}
