#ifndef PUDDI_H
#define PUDDI_H

#define _CRT_SECURE_NO_DEPRECATE
#define GLUT_DISABLE_ATEXIT_HACK
#define GLM_FORCE_RADIANS

#include "GlmHeaders.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

#include "Camera.h"
#include "Object.h"

namespace puddi
{
	class Object;
	class RenderGraph;
	class Camera;
	class DrawableObject;
	struct UpdateNode;
	struct ModelNode;
	enum ShadowMode : int;
	enum ShadowResolution : int;

	typedef int (*update_function)(void);
	typedef void (*draw_function)(void);
	typedef void (*init_function)(void);

	class Puddi
	{
	public:
		static float WorldSize;
		static float ViewDistance;
		static float FOV;

		static mat4 ProjectionMatrix;
		static mat4 CameraMatrix;
		static vec4 CameraPosition;

		static Camera *MainCamera;

		static std::string WindowTitleMessage;

		static int Init(float worldSize);

		static int Run();

		static void UpdateProjectionMatrixAndViewport();

		static Object* GetRootObject();

		static RenderGraph* GetDefaultRenderGraph();
		static RenderGraph* GetRenderGraph(size_t index);
		static size_t AddRenderGraph();

		static void RenderAll();

		static void ForceModelUpdate();

		// only enables full screen atm
		static void ToggleFullScreen();

        static void RegisterPostInitFunction(init_function f);
		static void RegisterUpdateFunction(update_function f);
		static void RegisterPreDrawFunction(draw_function f);
		static void RegisterDrawFunction(draw_function f);

        // must be called after Shader::Init (call from post_init function)
		static void EnableShadows(ShadowMode mode, ShadowResolution resolution);

		static void SetShadowLightPosition(const glm::vec3 &pos);
		static void SetShadowIgnoreObject(DrawableObject *o);

	private:
		static SDL_Window *window;
		static SDL_GLContext glcontext;

		static Object *rootObject;
		static UpdateNode *rootUpdateNode;
		static ModelNode *rootModelNode;

		static std::vector<RenderGraph*> renderGraphs;

        static std::vector<init_function> postInitFunctions;
		static std::vector<update_function> updateFunctions;
		static std::vector<draw_function> preDrawFunctions;
		static std::vector<draw_function> drawFunctions;

		static glm::vec3 shadowLightPosition;
		static DrawableObject *shadowIgnoreObject;

		static int update();

		static void preDraw();

		static void draw();

		// do this after all vertex data is loaded
		static void sendVertexDataToGPU();

		static int cleanup(int status_code);
	};
}

#endif
