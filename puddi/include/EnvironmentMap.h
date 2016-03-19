/**
 * @file EnvironmentMap.h
 * @author Alexander Bagnall
 * @brief Provides environment map functionality. (reflective surfaces)
 *
 * Contains functions for creating a cube map and rendering the
 * environment to it as seen from a given point.
 */

#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#define ENVIRONMENT_MAP_SIZE 1024

#include <GL/glew.h>
#include <glm/vec3.hpp>

namespace puddi
{
	class DrawableObject;

	namespace EnvironmentMap
	{
		void Init();

		GLuint GenerateCubeMap();

		void Render(GLuint targetCubeMap, const glm::vec3& sourcePos, DrawableObject* source, float zNear, float zFar);
	}
}

#endif
