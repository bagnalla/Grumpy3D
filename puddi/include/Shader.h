#ifndef SHADER_H
#define SHADER_H

#define BUFFER_OFFSET(offset)   ((GLvoid*) (offset))

#include <vector>
#include <unordered_map>
#include <string>
#include <GL/glew.h>
#include "GlmHeaders.h"

namespace puddi
{
	struct Material;
	enum ShadowMode : int;

	enum TextureSampler
	{
		TEXTURE_2D,
		TEXTURE_2D_BUMP,
		TEXTURE_CUBE,
		TEXTURE_CUBE_BUMP,
		TEXTURE_SHADOW_2D,
		TEXTURE_SHADOW_CUBE,
		TEXTURE_TERRAIN_1,
		TEXTURE_TERRAIN_BUMP_1,
		TEXTURE_TERRAIN_2,
		TEXTURE_TERRAIN_BUMP_2,
		TEXTURE_TERRAIN_3,
		TEXTURE_TERRAIN_BUMP_3,
		TEXTURE_TERRAIN_4,
		TEXTURE_TERRAIN_BUMP_4,
	};

	class Shader
	{
	public:
		static std::vector<vec4> Vertices;
		static std::vector<vec4> Normals;
		static std::vector<vec4> Tangents;
		static std::vector<vec4> Binormals;
		static std::vector<vec2> TextureCoordinates;

		static std::vector<uint> VertexIndices;

		static void Init();

		static void SetProgram(std::string programName);

		static void SetMaterialAmbient(const vec4& materialAmbient);
		static void SetMaterialDiffuse(const vec4& materialDiffuse);
		static void SetMaterialSpecular(const vec4& materialSpecular);
		static void SetMaterialShininess(float materialShininess);
		static void SetMaterial(const Material& material);
		static void SetLightSource(const mat4& lightSource);
		static void SetCameraPosition(const vec4& cameraPosition);
		static void SetModel(const mat4& model);
		static void SetCamera(const mat4& camera);
		static void SetProjection(const mat4& projection);
		static void SetLightProjection(const mat4& lightProj);
		static void SetEmissionColor(const vec4& emissionColor);
		static void SetTextureBlend(bool textureBlendb);
		static void SetShadowMode(ShadowMode shadowMode);
		static void SetShadowZRange(const vec2& shadowZRange);
		static void SetReflectiveCubeMap(bool b);

		// terrain uniforms
		static void SetGroundCoordZ(float z);
		static void SetTerrainScaleZ(float z);
		static void SetTerrainMaxHeight(int z);
		static void SetTexHeights(const vec4& texHeights);

		// either depth texture or depth cubemap
		static void BindShadowMap(GLuint shadowMap, ShadowMode mode);

	private:
		static GLuint vertexBuffer;
		static GLuint elementBuffer;
		static GLuint currentProgram;
		static std::string currentProgramName;

		static mat4 camera;
		static vec4 cameraPosition;
		static mat4 projection;
		static mat4 lightSource;
		static mat4 lightProjection;
		static ShadowMode shadowMode;
		static vec2 shadowZRange;

		static std::unordered_map<std::string, GLuint> nameToProgramMap;
		static std::unordered_map<GLuint, GLuint> programToVaoMap;
		static std::unordered_map<GLuint, std::unordered_map<std::string, GLuint>> programToUniformMap;

		static void initEmissiveColorProgram();
		static void initEmissiveTextureProgram();
		static void initEmissiveCubeMapProgram();
		static void initDepthProgram();
		static void initMaterialProgram();
		static void initMaterialBumpProgram();
		static void initTextureProgram();
		static void initTextureBumpProgram();
		static void initCubeMapProgram();
		static void initCubeMapBumpProgram();
		static void initTerrainTextureBumpProgram();

		static GLuint getUniform(GLuint program, std::string name);

		static int getUniformFromCurrentProgram(std::string uniformName);
	};
}

#endif
