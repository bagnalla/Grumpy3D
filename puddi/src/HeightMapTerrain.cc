#include "HeightMapTerrain.h"
#include "VertexMesh.h"
#include "TerrainVertexMesh.h"
#include "Shader.h"
#include <SDL2/SDL_image.h>
#include <iostream>

namespace puddi
{
	// PUBLIC

	TerrainVertexMesh HeightMapTerrain::CreateTerrainMeshFromFile(const char *fileName, float texWidth, float texHeight, float scaleZ)
	{
		SDL_Surface *surface = IMG_Load(fileName);

		if (surface == NULL)
		{
			std::cerr << "unable to load file " << fileName << std::endl;
			return TerrainVertexMesh(Material::None(), 0, 0);
		}

		unsigned char *pixels = (unsigned char*)surface->pixels;
		int bytesPerPixel = surface->format->BytesPerPixel;
		float maxHeight = FLT_MIN;

		std::vector<vec4> vertices;
		for (int i = 0; i < surface->h; ++i)
		{
			for (int j = 0; j < surface->w; ++j)
			{
				unsigned char height = pixels[(i * surface->w + j) * bytesPerPixel];
				if (height > maxHeight)
					maxHeight = height;
				vertices.push_back(vec4(i, j, height * scaleZ, 1.0f));
			}
		}

		std::vector<vec4> normals;
		std::vector<vec4> tangents;
		std::vector<vec4> binormals;
		for (int i = 0; i < surface->h; ++i)
		{
			for (int j = 0; j < surface->w; ++j)
			{
				if (i == 0 || i == surface->h - 1 || j == 0 || j == surface->w - 1)
				{
					normals.push_back(vec4(0.0f, 0.0f, 1.0f, 0.0f));
					tangents.push_back(vec4(1.0f, 0.0f, 0.0f, 0.0f));
					binormals.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
					continue;
				}

				vec3 v1 = vec3(vertices[(i - 1) * surface->w + j]);
				vec3 v2 = vec3(vertices[(i - 1) * surface->w + j + 1]);
				vec3 v3 = vec3(vertices[i * surface->w + j - 1]);
				vec3 v4 = vec3(vertices[i * surface->w + j]);
				vec3 v5 = vec3(vertices[i * surface->w + j + 1]);
				vec3 v6 = vec3(vertices[(i + 1) * surface->w + j - 1]);
				vec3 v7 = vec3(vertices[(i + 1) * surface->w + j]);

				vec3 tan1 = v4 - v3;
				vec3 binorm1 = v1 - v4;
				vec3 tan2 = v2 - v1;
				vec3 binorm2 = v1 - v4;
				vec3 tan3 = v5 - v4;
				vec3 binorm3 = v1 - v4;
				vec3 tan4 = v4 - v3;
				vec3 binorm4 = v3 - v6;
				vec3 tan5 = v7 - v6;
				vec3 binorm5 = v3 - v6;
				vec3 tan6 = v5 - v4;
				vec3 binorm6 = v4 - v7;

				vec3 n1 = normalize(cross(tan1, binorm1));
				vec3 n2 = normalize(cross(tan2, binorm2));
				vec3 n3 = normalize(cross(tan3, binorm3));
				vec3 n4 = normalize(cross(tan4, binorm4));
				vec3 n5 = normalize(cross(tan5, binorm5));
				vec3 n6 = normalize(cross(tan6, binorm6));

				normals.push_back(normalize(vec4((n1 + n2 + n3 + n4 + n5 + n6) / 6.0f, 0.0f)));
				tangents.push_back(normalize(vec4((tan1 + tan2 + tan3 + tan4 + tan5 + tan6) / 6.0f, 0.0f)));
				binormals.push_back(normalize(vec4((binorm1 + binorm2 + binorm3 + binorm4 + binorm5 + binorm6) / 6.0f, 0.0f)));
			}
		}
		for (int i = 0; i < surface->h; ++i)
		{
			for (int j = 0; j < surface->w; ++j)
			{
				if (i == 0 || j == surface->w - 1)
				{
					tangents.push_back(vec4(1.0f, 0.0f, 0.0f, 0.0f));
					binormals.push_back(vec4(0.0f, 1.0f, 0.0f, 0.0f));
					continue;
				}


			}
		}

		std::vector<vec2> textureCoords;
		if (texWidth == 0)
			texWidth = static_cast<float>(surface->w);
		if (texHeight == 0)
			texHeight = static_cast<float>(surface->h);
		for (int i = 0; i < surface->h; ++i)
		{
			for (int j = 0; j < surface->w; ++j)
			{
				//textureCoords.push_back(vec2(fmod(j, texWidth) / texWidth, fmod(i, texHeight) / texHeight));
				textureCoords.push_back(vec2(j / texWidth, i / texHeight));
			}
		}

		int vertexOffset = Shader::Vertices.size();
		int indexOffset = Shader::VertexIndices.size();

		std::vector<uint> indices;
		for (int i = 0; i < surface->h - 1; ++i)
		{
			for (int j = 0; j < surface->w; ++j)
			{
				indices.push_back(vertexOffset + i * surface->w + j);
				indices.push_back(vertexOffset + (i + 1) * surface->w + j);
			}
			indices.push_back(UINT_MAX);
		}

		Shader::Vertices.insert(Shader::Vertices.end(), vertices.begin(), vertices.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), textureCoords.begin(), textureCoords.end());
		Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());

		TerrainVertexMesh mesh = TerrainVertexMesh(Material::Plastic(vec4(0.5f, 0.5f, 0.5f, 1.0f)), indexOffset, indices.size());
		mesh.SetGroundCoordZ(0.0f);
		mesh.SetScaleZ(scaleZ);
		mesh.SetMaxHeight(maxHeight);
		mesh.SetSizeX(surface->w);
		mesh.SetSizeY(surface->h);

		// free the surface resource
		SDL_FreeSurface(surface);

		return mesh;
	}

	// PRIVATE
}
