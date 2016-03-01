#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#define ENVIRONMENT_MAP_SIZE 1024

#include <GL/glew.h>
#include <glm/vec3.hpp>

namespace puddi
{
	class DrawableObject;

	class EnvironmentMap
	{
	public:
		static void Init();

		static GLuint GenerateCubeMap();

		static void Render(GLuint targetCubeMap, const glm::vec3& sourcePos, DrawableObject* source, float zNear, float zFar);

	private:
		static GLuint frameBuffer;
		static GLuint depthBuffer;
	};
}

#endif
