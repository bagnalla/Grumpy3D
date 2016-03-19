#ifndef SHADOW_H
#define SHADOW_H

#define SHADOW_MAP_SIZE_DEFAULT SHADOW_RESOLUTION_MEDIUM

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

namespace puddi
{
	class DrawableObject;
	class VertexMesh;

	enum ShadowMode : int
	{
		SHADOW_MODE_NONE,
		SHADOW_MODE_UNI,
		SHADOW_MODE_OMNI
	};

	enum ShadowResolution : int
	{
		SHADOW_RESOLUTION_LOW = 512,
		SHADOW_RESOLUTION_MEDIUM = 1024,
		SHADOW_RESOLUTION_HIGH = 2048,
		SHADOW_RESOLUTION_EXTRA_HIGH = 4096
	};

	namespace Shadow
	{
		void Init();

		void RenderShadowOrthoMap(const glm::vec3& lightDir);

		void RenderShadowCubeMap(const glm::vec3& sourcePos, DrawableObject* source);

		void AddToDepthRenderList(VertexMesh *o);

		void RemoveFromDepthRenderList(VertexMesh *o);

		ShadowMode GetMode();
		void SetMode(ShadowMode mode);

		void SetZRange(float zNear, float zFar);

		void SetResolution(ShadowResolution res);
	};
}

#endif
