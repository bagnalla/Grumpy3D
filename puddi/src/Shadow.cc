#include "Shadow.h"
#include "DrawableObject.h"
#include "Shader.h"
#include "Puddi.h"
#include "RenderGraph.h"
#include <iostream>
#include <algorithm>
#include "Cube.h"

namespace puddi
{
    namespace Shadow
    {
        // PRIVATE
        namespace
        {
            GLuint frameBuffer;
            GLuint depthBuffer;
            GLuint depthCubeMap;
            vec2 zRange = vec2(1.0f, 2.0f);
            ShadowMode mode = SHADOW_MODE_NONE;
            int resolution = SHADOW_MAP_SIZE_DEFAULT;

            std::vector<VertexMesh*> depthRenderList;

            void configureDepthAndCubeMapBuffers()
            {
            }

            void renderDepthRenderList()
            {
                for (auto it = depthRenderList.begin(); it != depthRenderList.end(); ++it)
                {
                    auto o = *it;
                    if (o->GetRenderEnabled())
                        o->Draw();
                }
            }

            mat4 computeOrthoCamAndProjection(const glm::vec3 lightDir, mat4& cam)
            {
                const int NUM_CORNERS = 8;
                vec4 cornersInClipSpace[] =
                {
                    vec4(-1.0f, -1.0f, -1.0f, 1.0f),
                    vec4(-1.0f, 1.0f, -1.0f, 1.0f),
                    vec4(1.0f, 1.0f, -1.0f, 1.0f),
                    vec4(1.0f, -1.0f, -1.0f, 1.0f),
                    vec4(-1.0f, -1.0f, 1.0f, 1.0f),
                    vec4(-1.0f, 1.0f, 1.0f, 1.0f),
                    vec4(1.0f, 1.0f, 1.0f, 1.0f),
                    vec4(1.0f, -1.0f, 1.0f, 1.0f)
                };

                mat4 inverseProj = inverse(engine::ProjectionMatrix * engine::CameraMatrix);

                std::vector<vec4> cornersInWorldSpace;
                for (int i = 0; i < NUM_CORNERS; ++i)
                    cornersInWorldSpace.push_back(inverseProj * cornersInClipSpace[i]);
                for (int i = 0; i < NUM_CORNERS; ++i)
                    cornersInWorldSpace[i] = cornersInWorldSpace[i] / cornersInWorldSpace[i].w;
                for (int i = 0; i < NUM_CORNERS; ++i)
                    cornersInWorldSpace[i].w = 1.0f;

                vec4 sum = vec4(0.0f, 0.0f, 0.0f, 0.0f);
                for (int i = 0; i < NUM_CORNERS; ++i)
                    sum += cornersInWorldSpace[i];

                vec4 centroid = sum / static_cast<float>(NUM_CORNERS);

                cam = lookAt(vec3(centroid) + normalize(lightDir) * engine::ViewDistance * 1.25f, vec3(centroid), vec3(0.0f, 0.0f, 1.0f));

                std::vector<vec4> cornersInLightViewSpace;
                for (int i = 0; i < NUM_CORNERS; ++i)
                    cornersInLightViewSpace.push_back(cam * cornersInWorldSpace[i]);

                float minX, minY, minZ, maxX, maxY, maxZ;
                minX = minY = minZ = FLT_MAX;
                maxX = maxY = maxZ = FLT_MIN;
                for (int i = 0; i < NUM_CORNERS; ++i)
                {
                    vec4 corner = cornersInLightViewSpace[i];
                    if (corner.x < minX)
                        minX = corner.x;
                    if (corner.y < minY)
                        minY = corner.y;
                    if (corner.z < minZ)
                        minZ = corner.z;
                    if (corner.x > maxX)
                        maxX = corner.x;
                    if (corner.y > maxY)
                        maxY = corner.y;
                    if (corner.z > maxZ)
                        maxZ = corner.z;
                }

                return ortho(minX, maxX, minY, maxY, -maxZ, -minZ);
            }
        }

        // PUBLIC

        void Init()
        {
            // SET UP FRAME BUFFER OBJECT
            glDeleteFramebuffers(1, &frameBuffer);
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            glEnable(GL_DEPTH_TEST);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            // SET UP DEPTH BUFFER
            glDeleteTextures(1, &depthBuffer);
            glGenTextures(1, &depthBuffer);
            glBindTexture(GL_TEXTURE_2D, depthBuffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

            glBindTexture(GL_TEXTURE_2D, 0);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

            // Always check that our framebuffer is ok
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cerr << "Init() - frame buffer error\n";
            }

            // SET UP DEPTH CUBE MAP
            glDeleteTextures(1, &depthCubeMap);
            glGenTextures(1, &depthCubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, depthCubeMap, 0);

            // Always check that our framebuffer is ok
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cerr << "Init() - frame buffer error after depth cube map initialiation\n";
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void RenderShadowOrthoMap(const vec3& lightDir)
        {
            Shader::SetProgram("depth");

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

            glViewport(0, 0, resolution, resolution);

            mat4 cam;
            mat4 proj = computeOrthoCamAndProjection(lightDir, cam);

            //mat4 projection = ortho(-zRange.y / 2.0f, zRange.y / 2.0f, -zRange.y / 2.0f, zRange.y / 2.0f, -zRange.y, zRange.y);
            //mat4 projection = ortho(-zRange.y, zRange.y, -zRange.y, zRange.y, zRange.x, zRange.y);
            //mat4 projection = perspective(static_cast<float>(M_PI) / 2.0f, 1.0f, zRange.x, zRange.y);
            Shader::SetProjection(proj);

            Shader::SetLightProjection(proj * cam);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(cam);
            renderDepthRenderList();

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            Shader::SetCamera(engine::CameraMatrix);
        }

        void RenderShadowCubeMap(const vec3& sourcePos, DrawableObject* source)
        {
            Shader::SetProgram("depth");

            if (source != NULL)
                source->DisableRender();

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

            glViewport(0, 0, resolution, resolution);

            mat4 projection = perspective(static_cast<float>(M_PI) / 2.0f, 1.0f, zRange.x, zRange.y);
            Shader::SetProjection(projection);

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, depthCubeMap, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
            renderDepthRenderList();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, depthCubeMap, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
            renderDepthRenderList();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, depthCubeMap, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
            renderDepthRenderList();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, depthCubeMap, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
            renderDepthRenderList();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, depthCubeMap, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)));
            renderDepthRenderList();

            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, depthCubeMap, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));
            renderDepthRenderList();

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            Shader::SetCamera(engine::CameraMatrix);

            if (source != NULL)
                source->EnableRender();
        }

        void AddToDepthRenderList(VertexMesh *o)
        {
            auto it = std::find(depthRenderList.begin(), depthRenderList.end(), o);
            if (it == depthRenderList.end())
                depthRenderList.push_back(o);
        }

        void RemoveFromDepthRenderList(VertexMesh *o)
        {
            auto it = std::find(depthRenderList.begin(), depthRenderList.end(), o);
            if (it != depthRenderList.end())
                depthRenderList.erase(it);
        }

        ShadowMode GetMode()
        {
            return mode;
        }

        void SetMode(ShadowMode m)
        {
            mode = m;

            Shader::SetShadowMode(mode);

            if (mode == SHADOW_MODE_UNI)
                Shader::BindShadowMap(depthBuffer, mode);
            else
                Shader::BindShadowMap(depthCubeMap, mode);
        }

        void SetZRange(float zNear, float zFar)
        {
            zRange = vec2(zNear, zFar);
            Shader::SetShadowZRange(zRange);
        }

        void SetResolution(ShadowResolution res)
        {
            resolution = res;
            Init();
        }
    }
}
