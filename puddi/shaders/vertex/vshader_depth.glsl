attribute vec4 vPosition;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	gl_Position = projection * camera * model * vPosition;
}
