#include "Puddi.h"
#include "RenderGraph.h"
#include "Rectangle.h"
#include "Cube.h"
#include "Sphere.h"
#include "Skybox.h"
#include "LightSource.h"
#include "Texture.h"
#include "Shadow.h"
#include "EnvironmentMap.h"
#include "Schematic.h"
#include "Shader.h"
#include "Util.h"
#include "FpsTracker.h"
#include "Font.h"
#include "DrawableObject.h"
//#include <SDL2/SDL_ttf.h>
#include <iostream>

using namespace std;

namespace puddi
{
	// PUBLIC

	float Puddi::WorldSize;
	float Puddi::ViewDistance;
	float Puddi::FOV = static_cast<float>(M_PI / 4.0f);

	mat4 Puddi::ProjectionMatrix;
	mat4 Puddi::CameraMatrix;
	vec4 Puddi::CameraPosition;

	Camera* Puddi::MainCamera;

	std::string Puddi::WindowTitleMessage = "";

	int Puddi::Init(float worldSize)
	{
		srand(static_cast<unsigned int>(time(NULL)));

		Puddi::WorldSize = worldSize;

		if (int status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
		{
			std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return status;
		}

		//if (TTF_Init())
		//{
			//std::cerr << "Failed to initialize TTF. Error: " << TTF_GetError() << std::endl;
		//}

		// 4x AA
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		window = SDL_CreateWindow(
			"Puddi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		// Create an OpenGL context associated with the window.
		glcontext = SDL_GL_CreateContext(window);

		// enable AA
		glEnable(GL_MULTISAMPLE);

		//SDL_GL_MakeCurrent(window, glcontext);
		//glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "GLEW failed to load.\n";
			exit(1);
		}

		if (SDL_SetRelativeMouseMode(SDL_TRUE) == -1)
			std::cerr << "unable to set relative mouse mode.\n";

		// default render graph
		renderGraphs.push_back(new RenderGraph());

		Rectangle::Init();
		Cube::Init();
		Sphere::Init();
		Skybox::Init();
		LightSource::Init();
		Shadow::Init();
		EnvironmentMap::Init();
		Schematic::Init();
		Font::Init();

		rootUpdateNode = rootObject->GetUpdateNode();
		//rootUpdateNode->parallel = true;

		rootModelNode = rootObject->GetModelNode();

		MainCamera = new Camera(rootObject);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		//glBlendFunc(GL_ONE, GL_ONE);

		if (GLEW_VERSION_3_1)
		{
			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(UINT_MAX);
		}

		Puddi::ViewDistance = Puddi::WorldSize * 1.6f;

		// set default clear color
		glClearColor(0.5, 0.5, 1.0, 1.0);

		// initialize projection matrix
		UpdateProjectionMatrixAndViewport();

		// SET SHADOW NEAR AND FAR PLANES
		Shadow::SetZRange(2.0f, Puddi::ViewDistance);

		// enable seamless cube maps
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		// start texture loading
		Texture::InitTextureLoading();

		std::cerr << "Puddi: initialization complete. Waiting for call to Run() ...\n";

		return 0;
	}

	int Puddi::Run()
	{
		// end texture loading
		Texture::EndTextureLoading();

		sendVertexDataToGPU();

		// call post-init functions
		for (auto it = postInitFunctions.begin(); it != postInitFunctions.end(); ++it)
			(*it)();

		int status_code;

		std::cerr << "Puddi: entering main loop\n";

		while (true)
		{
			if ((status_code = update()))
				return cleanup(status_code);

			// call additional update functions
			for (auto it = updateFunctions.begin(); it != updateFunctions.end(); ++it)
			{
				if ((status_code = (*it)()))
					return cleanup(status_code);
			}

			// call pre-draw functions
			for (auto it = preDrawFunctions.begin(); it != preDrawFunctions.end(); ++it)
				(*it)();

			preDraw();

			draw();
		}
	}
	void Puddi::UpdateProjectionMatrixAndViewport()
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		Puddi::ProjectionMatrix = perspective(static_cast<float>(M_PI) / 4.0f, w / static_cast<float>(h), 1.0f, Puddi::ViewDistance);
		Shader::SetProjection(Puddi::ProjectionMatrix);

		glViewport(0, 0, w, h);
	}

	Object* Puddi::GetRootObject()
	{
		return rootObject;
	}

	RenderGraph* Puddi::GetDefaultRenderGraph()
	{
		return renderGraphs[0];
	}

	RenderGraph* Puddi::GetRenderGraph(size_t index)
	{
		if (index >= renderGraphs.size())
			return nullptr;
		return renderGraphs[index];
	}

	size_t Puddi::AddRenderGraph()
	{
		renderGraphs.push_back(new RenderGraph());
		return renderGraphs.size() - 1;
	}

	void Puddi::RenderAll()
	{
		for (auto it = renderGraphs.begin(); it != renderGraphs.end(); ++it)
			(*it)->Render();
	}

	void Puddi::ForceModelUpdate()
	{
        rootModelNode->Update();
	}

	void Puddi::ToggleFullScreen()
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

    void Puddi::RegisterPostInitFunction(init_function f)
	{
		postInitFunctions.push_back(f);
	}

	void Puddi::RegisterUpdateFunction(update_function f)
	{
		updateFunctions.push_back(f);
	}

	void Puddi::RegisterPreDrawFunction(draw_function f)
	{
		preDrawFunctions.push_back(f);
	}

	void Puddi::RegisterDrawFunction(draw_function f)
	{
		drawFunctions.push_back(f);
	}

    void Puddi::EnableShadows(ShadowMode mode, ShadowResolution resolution)
    {
        Shadow::SetResolution(resolution);

        // initial call to these so that the shadow map binding will
        // work when you enable shadows
        if (mode == SHADOW_MODE_UNI)
            Shadow::RenderShadowOrthoMap(vec3(1.0f, 0.0f, 0.0f));
        else
            Shadow::RenderShadowCubeMap(vec3(1.0f, 0.0f, 0.0f), shadowIgnoreObject);

        Shadow::SetMode(mode);
    }

    void Puddi::SetShadowLightPosition(const glm::vec3 &pos)
    {
        shadowLightPosition = pos;
    }

    void Puddi::SetShadowIgnoreObject(DrawableObject *o)
    {
        shadowIgnoreObject = o;
    }

	// PRIVATE

	SDL_Window* Puddi::window;
	SDL_GLContext Puddi::glcontext;

	Object* Puddi::rootObject = new Object(nullptr);
	UpdateNode* Puddi::rootUpdateNode;
	ModelNode* Puddi::rootModelNode;

	vector<RenderGraph*> Puddi::renderGraphs;

    std::vector<init_function> Puddi::postInitFunctions;
	std::vector<update_function> Puddi::updateFunctions;
	std::vector<draw_function> Puddi::preDrawFunctions;
	std::vector<draw_function> Puddi::drawFunctions;

	glm::vec3 Puddi::shadowLightPosition;
	DrawableObject* Puddi::shadowIgnoreObject = nullptr;

	int Puddi::update()
	{
		FpsTracker::Update();

		SDL_SetWindowTitle(window, Util::ToString(FpsTracker::GetFps()).c_str());

		rootModelNode->Update();

		rootUpdateNode->Update();
		//std::cout << rootModelNode->children.size() << std::endl;

		return 0;
	}

	void Puddi::preDraw()
	{
        if (Shadow::GetMode() == SHADOW_MODE_UNI)
            Shadow::RenderShadowOrthoMap(shadowLightPosition);
        else if (Shadow::GetMode() == SHADOW_MODE_OMNI)
            Shadow::RenderShadowCubeMap(shadowLightPosition, shadowIgnoreObject);
	}

	void Puddi::draw()
	{
		UpdateProjectionMatrixAndViewport();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderAll();

		// call additional draw functions
        for (auto it = drawFunctions.begin(); it != drawFunctions.end(); ++it)
            (*it)();

		SDL_GL_SwapWindow(window);
		glFlush();
	}

	int Puddi::cleanup(int status_code)
	{
		Schematic::Cleanup();

		SDL_GL_DeleteContext(glcontext);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return status_code;
	}

	void Puddi::sendVertexDataToGPU()
	{
		Shader::Init();

		// clear all vertex data on cpu side
		Shader::Vertices.clear();
		Shader::Normals.clear();
		Shader::Tangents.clear();
		Shader::Binormals.clear();
		Shader::TextureCoordinates.clear();
		Shader::VertexIndices.clear();
		Shader::Vertices.resize(0);
		Shader::Normals.resize(0);
		Shader::Tangents.resize(0);
		Shader::Binormals.resize(0);
		Shader::TextureCoordinates.resize(0);
		Shader::VertexIndices.resize(0);
	}

}
