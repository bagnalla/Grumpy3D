attribute vec4 vPosition;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	// compute gl_Position
	gl_Position = projection * camera * model * vPosition;
}
