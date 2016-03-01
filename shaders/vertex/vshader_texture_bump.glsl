attribute vec4 vPosition;
attribute vec4 vNormal;
attribute vec4 vTangent;
attribute vec4 vBinormal;
attribute vec2 vTextureCoordinate;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec2 fTextureCoord;
varying mat4 inverseTBN;
varying vec3 shadowCoordDepth;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 lightSource;
uniform vec4 cameraPosition;
uniform int shadowMode;
uniform mat4 lightProjection;

void main()
{
	// compute vPosition in world space
	vec4 vPositionWorld = model * vPosition;
	
	inverseTBN = mat4(vTangent, vBinormal, vNormal, vec4(0.0, 0.0, 0.0, 0.0));
	
	// compute normal in world space
	N = (model * vNormal).xyz;

	// compute eye direction
	E = (cameraPosition - vPositionWorld).xyz;

	if (lightSource[3].w == 0.0)
		L = lightSource[3].xyz;
	else
		L = (lightSource[3] - vPositionWorld).xyz;

	if (shadowMode == 1)
	{
		vec3 vPositionLight = (lightProjection * vPositionWorld).xyz;
		float bias = 0.005*tan(acos(dot(normalize(N), normalize(L))));
		bias = clamp(bias, 0, 0.01);
		shadowCoordDepth = vec3((vPositionLight.x + 1.0) / 2.0, (vPositionLight.y + 1.0) / 2.0, (vPositionLight.z + 1.0) / 2.0 - bias);
	}
	
	// pass texture coordinates to be interpolated over fragments
	fTextureCoord = vec2((vTextureCoordinate.x), (vTextureCoordinate.y));
	
	// compute gl_Position
	gl_Position = projection * camera * vPositionWorld;
}
