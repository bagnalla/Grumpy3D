/**
 * @file InitShader.h
 * @author Alexander Bagnall
 * @brief Provides functions for compiling and attaching shader programs.
 *
 * This is taken from Chelberg's CS 4250 class. I think it is from the
 * Interactive Computer Graphics 6th Edition (Angel, Shreiner) textbook.
 */

#ifndef INITSHADER_H
#define INITSHADER_H

namespace puddi
{
	char* readShaderSource(const char* shaderFile);

	// Create a GLSL program object from vertex and fragment shader files
	GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

	// Create a GLSL program object from vertex shader file
	GLuint InitShader(const char* vShaderFile);
}

#endif
