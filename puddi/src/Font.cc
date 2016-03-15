#include "Font.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"
#include "DrawableObject.h"
#include <iostream>

using namespace std;

namespace puddi
{
	// PUBLIC

	void Font::Init()
	{
        Texture::LoadTexture("myfont", "puddi/textures/font_60_tolerance.png");

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

    vector<DrawableObject*> Font::CreateGlyphString(Object *rootObject, std::string font, string str, float scale)
    {
        vector<DrawableObject*> glyphs;

        vec4 initialPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        vec4 posCursor = initialPos;
        float charWidth = 0.5f * scale;
        float tabSpace = 4.0f * charWidth;
        for (size_t i = 0; i < str.size(); ++i)
        {
            char c = str[i];

            // tab
            if (c == 9)
            {
                glyphs.push_back(nullptr);
                posCursor += vec4(tabSpace, 0.0f, 0.0f, 0.0f);
                continue;
            }

            // carriage return
            if (c == 13 || c == 10)
            {
                glyphs.push_back(nullptr);
                posCursor += vec4(0.0f, 0.0f, -1.0f, 0.0f);
                posCursor.x = initialPos.x;
                continue;
            }

//            if (c == 32)
//            {
//                glyphs.push_back(nullptr);
//                posCursor += vec4(charWidth, 0.0f, 0.0f, 0.0f);
//                continue;
//            }

            // ignore weird characters
            if (c < 32 || c > 126)
            {
                glyphs.push_back(nullptr);
                continue;
            }

            DrawableObject *glyph = new DrawableObject(rootObject, VertexMesh::GetVertexMeshPrototypeByName(std::string(1, c)));
            glyph->SetTexture(Texture::GetTextureByName(font));
            glyph->RotateX(static_cast<float>(M_PI / 2.0));
            glyph->SetPosition(posCursor);
            glyph->SetEmissive(true);
            glyph->SetRenderGraph(1);
            glyph->SetScaleX(charWidth);
            glyph->SetScaleY(scale);
            glyph->SetScaleZ(scale);
            glyphs.push_back(glyph);

            posCursor += vec4(charWidth, 0.0f, 0.0f, 0.0f);
        }

        return glyphs;
    }

	// PRIVATE

	std::vector<Rectangle*> Font::characterRectangles;
}
