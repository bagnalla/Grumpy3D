varying vec3 cubeMapCoord;

uniform samplerCube cubeMap;

void main()
{
	gl_FragColor = vec4(textureCube(cubeMap, cubeMapCoord).xyz, 1.0);
}
