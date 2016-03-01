#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <string>

namespace puddi
{
	class Texture
	{
	public:
		static void InitTextureLoading();
		static void EndTextureLoading();

		static GLuint LoadTexture(const char *name, const char *filePath, const char *bumpPath = nullptr);
		static GLuint LoadBumpMap(const char *name, const char *bumpPath);
		static GLuint LoadCubeMap(const char *name, const char *filePath, const char *bumpPath = nullptr);
		static GLuint LoadCubeMapMirrored(const char *name, const char *filePath, const char *bumpPath = nullptr);

		static GLuint GetTextureByName(std::string name);
		static GLuint GetBumpMapByName(std::string name); // only works if it was loaded by name
		static GLuint GetTextureByFile(std::string path);
		static GLuint GetBumpMapByTexture(GLuint tex);

		static GLuint GetCubeMapByName(std::string name);
		static GLuint GetCubeMapByFile(std::string path);
		static GLuint GetBumpMapByCubeMap(GLuint cubeMap);

		static void DestroyTexture(const std::string &name, const std::string &path = "");

	private:
		static std::unordered_map<std::string, GLuint> textureMap;
		static std::unordered_map<std::string, GLuint> textureFileMap;
		static std::unordered_map<GLuint, GLuint> bumpMapMap;
		static std::unordered_map<std::string, GLuint> bumpMapNameMap;

		static std::unordered_map<std::string, GLuint> cubeMapMap;
		static std::unordered_map<std::string, GLuint> cubeMapFileMap;
		static std::unordered_map<GLuint, GLuint> cubeBumpMapMap;

		static GLuint loadTexture(const char *path);
		//static GLuint loadTextureFromSurface(SDL_Surface *surface, GLint internalFormat, GLint format);
		static GLuint loadCubeMap(const char *path);
		static GLuint loadCubeMapMirrored(const char *path);
	};
}

#endif
