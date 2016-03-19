#include "EnvironmentMap.h"
#include "DrawableObject.h"
#include "Shader.h"
#include "Puddi.h"
#include "RenderGraph.h"
#include "Shadow.h"

namespace puddi
{
    namespace EnvironmentMap
    {
        // PRIVATE
        namespace
        {
            GLuint frameBuffer;
            GLuint depthBuffer;
        }

        // PUBLIC

        void Init()
        {
            // SET UP FRAME BUFFER FOR RENDERING TO CUBE MAP
            frameBuffer = 0;
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            glEnable(GL_DEPTH_TEST);

            // SET UP DEPTH BUFFER FOR FBO
            glGenRenderbuffers(1, &depthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        GLuint GenerateCubeMap()
        {
            GLuint cubeMap;

            // SET UP COLOR CUBE MAP
            glGenTextures(1, &cubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            return cubeMap;
        }

        void Render(GLuint targetCubeMap, const vec3& sourcePos, DrawableObject* source, float zNear, float zFar)
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

            glViewport(0, 0, ENVIRONMENT_MAP_SIZE, ENVIRONMENT_MAP_SIZE);

            if (source != nullptr)
                source->DisableRender();

            mat4 projection = perspective(static_cast<float>(M_PI) / 2.0f, 1.0f, zNear, zFar);
            Shader::SetProjection(projection);
            ShadowMode shadowMode = Shadow::GetMode();
            Shader::SetShadowMode(SHADOW_MODE_NONE);

            glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, targetCubeMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
            engine::RenderAll();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, targetCubeMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
            engine::RenderAll();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, targetCubeMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
            engine::RenderAll();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, targetCubeMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
            engine::RenderAll();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, targetCubeMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)));
            engine::RenderAll();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, targetCubeMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));
            engine::RenderAll();

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            glBindTexture(GL_TEXTURE_CUBE_MAP, targetCubeMap);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            if (source != nullptr)
                source->EnableRender();

            Shader::SetShadowMode(shadowMode);
            Shader::SetCamera(engine::CameraMatrix);
        }
    }
}
