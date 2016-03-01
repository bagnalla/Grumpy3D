attribute vec4 vPosition;
attribute vec2 vTextureCoordinate;

varying vec2 fTextureCoord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	// pass texture coordinates to be interpolated over the fragments
	fTextureCoord = vec2((vTextureCoordinate.x), (vTextureCoordinate.y));
	
	// compute gl_Position
	gl_Position = projection * camera * model * vPosition;
}
