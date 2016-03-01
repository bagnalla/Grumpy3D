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

	class Shadow
	{
	public:
		static void Init();

		static void RenderShadowOrthoMap(const glm::vec3& lightDir);

		static void RenderShadowCubeMap(const glm::vec3& sourcePos, DrawableObject* source);

		static void AddToDepthRenderList(VertexMesh *o);

		static void RemoveFromDepthRenderList(VertexMesh *o);

		static ShadowMode GetMode();
		static void SetMode(ShadowMode mode);

		static void SetZRange(float zNear, float zFar);

		static void SetResolution(ShadowResolution res);

	private:
		static GLuint frameBuffer;
		static GLuint depthBuffer; // for shadow ortho map
		static GLuint depthCubeMap; // for shadow cube map
		static glm::vec2 zRange;
		static ShadowMode mode;
		static int resolution;

		static std::vector<VertexMesh*> depthRenderList;

		static void configureDepthAndCubeMapBuffers();

		static void renderDepthRenderList();

		static glm::mat4 computeOrthoCamAndProjection(const glm::vec3 lightDir, glm::mat4& cam);
	};
}

#endif
