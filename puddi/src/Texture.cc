#include "Texture.h"
#include "SDLUtil.h"
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2_gfx/SDL2_rotozoom.h>
#include <iostream>
#include <string>
#include <cstdio>

namespace puddi
{
    namespace Texture
    {
        // PRIVATE
        namespace
        {
            std::unordered_map<std::string, GLuint> textureMap;
            std::unordered_map<std::string, GLuint> textureFileMap;
            std::unordered_map<GLuint, GLuint> bumpMapMap;
            std::unordered_map<std::string, GLuint> bumpMapNameMap;

            std::unordered_map<std::string, GLuint> cubeMapMap;
            std::unordered_map<std::string, GLuint> cubeMapFileMap;
            std::unordered_map<GLuint, GLuint> cubeBumpMapMap;

        //	GLuint loadTexture(const char *path)
        //	{
        //		if (path == nullptr)
        //			return 0;
        //
        //		SDL_Surface *surface = IMG_Load(path);
        //
        //		if (surface == NULL)
        //		{
        //			std::cerr << "unable to load file " << path << ". error: " << IMG_GetError() << std::endl;
        //			return 0;
        //		}
        //
        //		GLint internalFormat = GL_RGB;
        //		if (surface->format->BytesPerPixel == 4)
        //			internalFormat = GL_RGBA;
        //
        //		GLint format;
        //		if (strcmp(strrchr(path, '.'), ".tga"))
        //		{
        //			format = GL_RGB;
        //			if (surface->format->BytesPerPixel == 4)
        //				format = GL_RGBA;
        //		}
        //		else
        //		{
        //			format = GL_BGR;
        //			if (surface->format->BytesPerPixel == 4)
        //				format = GL_BGRA;
        //		}
        //
        //		GLuint tex = loadTextureFromSurface(surface, internalFormat, format);
        //
        //		// free the surface resource
        //		SDL_FreeSurface(surface);
        //
        //		return tex;
        //	}

        GLuint loadTexture(const char *path)
            {
                if (path == nullptr)
                    return 0;

                SDL_Surface *surface = IMG_Load(path);

                if (surface == NULL)
                {
                    std::cerr << "unable to load file " << path << ". error: " << IMG_GetError() << std::endl;
                    return 0;
                }

                GLuint texture;

                glGenTextures(1, &texture);             // Generate a texture
                glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily

                if (texture == 0)
                {
                    std::cerr << "unable to get a free texture ID from opengl for file " << path << std::endl;
                    return texture;
                }

                GLint internalFormat = GL_RGB;
                if (surface->format->BytesPerPixel == 4)
                    internalFormat = GL_RGBA;

                GLint format;
                if (strcmp(strrchr(path, '.'), ".tga"))
                {
                    format = GL_RGB;
                    if (surface->format->BytesPerPixel == 4)
                        format = GL_RGBA;
                }
                else
                {
                    format = GL_BGR;
                    if (surface->format->BytesPerPixel == 4)
                        format = GL_BGRA;
                }

                //internalFormat = format = GL_BGRA;

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                // Generate mipmaps, by the way.
                glGenerateMipmap(GL_TEXTURE_2D);

                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

                // Unbind the texture
                glBindTexture(GL_TEXTURE_2D, 0);

                // free the surface resource
                SDL_FreeSurface(surface);

                return texture;
            }

        //	GLuint loadTextureFromSurface(SDL_Surface *surface, GLint internalFormat, GLint format)
        //	{
        //		GLuint texture;
        //
        //		glGenTextures(1, &texture);             // Generate a texture
        //		glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily
        //
        //		if (texture == 0)
        //		{
        //			std::cerr << "unable to get a free texture ID from opengl\n";
        //			return texture;
        //		}
        //
        //		//return createTextureFromBitmap(surface->pixels, surface->w, surface->h, format);
        //
        //		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
        //
        //		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        //		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        //		// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
        //		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //		// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
        //		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //		// Generate mipmaps, by the way.
        //		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        //		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        //		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //		glGenerateMipmap(GL_TEXTURE_2D);
        //
        //		// Unbind the texture
        //		glBindTexture(GL_TEXTURE_2D, 0);
        //
        //		return texture;
        //	}

            GLuint loadCubeMap(const char *path)
            {
                if (path == nullptr)
                    return 0;

                SDL_Surface *surface = IMG_Load(path);

                if (surface == NULL)
                {
                    std::cerr << "unable to load file " << path << std::endl;
                    return 0;
                }

        //        GLint internalFormat = GL_RGB8;
        //		if (surface->format->BytesPerPixel == 4)
        //			internalFormat = GL_RGBA8;

        //		GLint format;
        //		if (strcmp(strrchr(path, '.'), ".tga"))
        //		{
        //			format = GL_RGB;
        //			if (surface->format->BytesPerPixel == 4)
        //				format = GL_RGBA;
        //		}
        //		else
        //		{
        //			format = GL_BGR;
        //			if (surface->format->BytesPerPixel == 4)
        //				format = GL_BGRA;
        //		}

                GLuint cubeMap;
                glGenTextures(1, &cubeMap);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                SDL_Rect rect;
                rect.w = surface->w / 4;
                rect.h = surface->h / 3;
                /*Uint32 rmask, gmask, bmask, amask;
                #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                rmask = 0xff000000;
                gmask = 0x00ff0000;
                bmask = 0x0000ff00;
                amask = 0x000000ff;
                #else
                rmask = 0x000000ff;
                gmask = 0x0000ff00;
                bmask = 0x00ff0000;
                amask = 0xff000000;
                #endif
                SDL_Surface *subSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, rmask, gmask, bmask, amask);*/
                SDL_Surface *subSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);

                // LEFT
                rect.x = 0;
                rect.y = rect.h;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                //subSurfaceRotated = rotozoomSurface(subSurface, 90.0, 1.0, 0);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // FRONT
                rect.x = rect.w;
                rect.y = rect.h;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // RIGHT
                rect.x = rect.w * 2;
                rect.y = rect.h;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                //subSurfaceRotated = rotozoomSurface(subSurface, 90.0, 1.0, 0);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // BACK
                rect.x = rect.w * 3;
                rect.y = rect.h;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // TOP
                rect.x = rect.w;
                rect.y = 0;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // BOTTOM
                rect.x = rect.w;
                rect.y = rect.h * 2;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // Unbind the texture
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

                // free the surface resources
                SDL_FreeSurface(surface);
                SDL_FreeSurface(subSurface);

                return cubeMap;
            }

            GLuint loadCubeMapMirrored(const char *path)
            {
                if (path == nullptr)
                    return 0;

                SDL_Surface *surface = IMG_Load(path);

                if (surface == NULL)
                {
                    std::cerr << "unable to load file " << path << std::endl;
                    return 0;
                }

                GLuint cubeMap;
                glGenTextures(1, &cubeMap);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                SDL_Rect rect;
                rect.w = surface->w / 4;
                rect.h = surface->h / 3;
                SDL_Surface *subSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);

                // LEFT
                rect.x = 0;
                rect.y = rect.h;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                SDLUtil::mirrorSurfaceX(subSurface);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // FRONT
                rect.x = rect.w;
                rect.y = rect.h;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                SDLUtil::mirrorSurfaceX(subSurface);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // RIGHT
                rect.x = rect.w * 2;
                rect.y = rect.h;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                SDLUtil::mirrorSurfaceX(subSurface);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // BACK
                rect.x = rect.w * 3;
                rect.y = rect.h;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                SDLUtil::mirrorSurfaceX(subSurface);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // TOP
                rect.x = rect.w;
                rect.y = 0;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                SDLUtil::mirrorSurfaceX(subSurface);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // BOTTOM
                rect.x = rect.w;
                rect.y = rect.h * 2;
                SDL_BlitSurface(surface, &rect, subSurface, NULL);
                SDLUtil::mirrorSurfaceX(subSurface);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

                // Unbind the texture
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

                // free the surface resources
                SDL_FreeSurface(surface);
                SDL_FreeSurface(subSurface);

                return cubeMap;
            }
        }
        // PUBLIC

        void InitTextureLoading()
        {
            IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
        }

        void EndTextureLoading()
        {
            // release texture loading stuff
            IMG_Quit();
        }

        GLuint LoadTexture(const char *name, const char *filePath, const char *bumpPath)
        {
            GLuint tex = loadTexture(filePath);
            if (tex != 0)
            {
                textureMap.emplace(name, tex);
                GLuint bump = loadTexture(bumpPath);
                if (bump != 0)
                    bumpMapMap.emplace(tex, bump);
            }
            return tex;
        }

        GLuint LoadBumpMap(const char *name, const char *bumpPath)
        {
            GLuint bump = loadTexture(bumpPath);
            if (bump != 0)
                bumpMapNameMap.emplace(name, bump);
            return bump;
        }

        GLuint LoadCubeMap(const char *name, const char *filePath, const char *bumpPath)
        {
            GLuint cubeMap = loadCubeMap(filePath);
            if (cubeMap != 0)
            {
                cubeMapMap.emplace(name, cubeMap);
                GLuint bump = loadCubeMap(bumpPath);
                if (bump != 0)
                    cubeBumpMapMap.emplace(cubeMap, bump);
            }
            return cubeMap;
        }

        GLuint LoadCubeMapMirrored(const char *name, const char *filePath, const char *bumpPath)
        {
            GLuint cubeMap = loadCubeMapMirrored(filePath);
            if (cubeMap != 0)
            {
                cubeMapMap.emplace(name, cubeMap);
                GLuint bump = loadCubeMapMirrored(bumpPath);
                if (bump != 0)
                    cubeBumpMapMap.emplace(cubeMap, bump);
            }
            return cubeMap;
        }

        GLuint GetTextureByName(std::string name)
        {
            return textureMap[name];
        }

        GLuint GetBumpMapByName(std::string name)
        {
            return bumpMapNameMap[name];
        }

        GLuint GetTextureByFile(std::string path)
        {
            return textureFileMap[path];
        }

        GLuint GetBumpMapByTexture(GLuint tex)
        {
            if (bumpMapMap.find(tex) != bumpMapMap.end())
                return bumpMapMap[tex];
            else
                return 0;
        }

        GLuint GetCubeMapByName(std::string name)
        {
            return cubeMapMap[name];
        }

        GLuint GetCubeMapByFile(std::string path)
        {
            return cubeMapFileMap[path];
        }

        GLuint GetBumpMapByCubeMap(GLuint cubeMap)
        {
            if (cubeBumpMapMap.find(cubeMap) != cubeBumpMapMap.end())
                return cubeBumpMapMap[cubeMap];
            else
                return 0;
        }

        void DestroyTexture(const std::string &name, const std::string &path)
        {
            GLuint tex = GetTextureByName(name);
            glDeleteTextures(1, &tex);
            textureMap.erase(name);
            if (path != "")
                textureFileMap.erase(path);
        }
    }
}
