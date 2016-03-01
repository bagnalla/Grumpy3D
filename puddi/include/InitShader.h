#ifndef INITSHADER_H
#define INITSHADER_H

namespace puddi
{
	static char* readShaderSource(const char* shaderFile);

	// Create a GLSL program object from vertex and fragment shader files
	GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

	// Create a GLSL program object from vertex shader file
	GLuint InitShader(const char* vShaderFile);
}

#endif
