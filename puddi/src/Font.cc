#include "Font.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"

#include <iostream>

using namespace std;

namespace puddi
{
	// PUBLIC

	void Font::Init()
	{
        Texture::LoadTexture("myfont", "textures/font_60_tolerance.png");

        std::vector<vec4> rect = Geometry::CreateRectangle(1.0);
		std::vector<vec4> normals({ vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0) });
		std::vector<vec4> tangents({ vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0) });
		std::vector<vec4> binormals({ vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0) });

        int rows = 10;
        int cols = 10;
        float tWidth = 1.0f / cols;
        float tHeight = 1.0f / rows;

		for (int i = 0; i < rows; ++i)
		{
            for (int j = 0; j < cols; ++j)
            {
                vec2 texCoordsArray[] = { vec2(j * tWidth, i * tHeight),
                    vec2(j * tWidth + tWidth, i * tHeight),
                    vec2(j * tWidth, i * tHeight + tHeight),
                    vec2(j * tWidth + tWidth, i * tHeight + tHeight)
                };
                std::vector<vec2> texCoords(texCoordsArray, texCoordsArray + sizeof(texCoordsArray) / sizeof(texCoordsArray[0]));

                int vertexOffset = Shader::Vertices.size();
                int indexOffset = Shader::VertexIndices.size();
                int indexCount = rect.size();

                std::vector<uint> indices;
                for (int i = 0; i < indexCount; ++i)
                    indices.push_back(vertexOffset + i);

                Shader::Vertices.insert(Shader::Vertices.end(), rect.begin(), rect.end());
                Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
                Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
                Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
                Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
                Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());

				//std::cout << "adding " << std::string(1, i * cols + j + 32) << std::endl;
                VertexMesh::AddVertexMeshPrototype(std::string(1, i * cols + j + 32), /*Material::None()*/Material::Plastic(vec4(1.0f, 1.0f, 1.0f, 1.0f)), indexOffset, indexCount, true);
            }
		}
	}


	// PRIVATE

	std::vector<Rectangle*> Font::characterRectangles;
}
