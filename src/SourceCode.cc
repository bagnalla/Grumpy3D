#include "SourceCode.h"
#include "Util.h"
#include "GlmHeaders.h"
#include "Texture.h"
#include "Font.h"

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

    SourceCode::SourceCode(Object *par, const string &path, const std::string &font) : DrawableObject(par)
    {
        auto chars = Util::ReadAllBytes(path);
        characters = string(chars.data());
        this->font = font;
        //createGlyphs();
        glyphs = Font::CreateGlyphString(this, font, characters);
    }

    // PRIVATE

    void SourceCode::createGlyphs()
    {
        vec4 initialPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        vec4 posCursor = initialPos;
        float charWidth = 0.5f;
        float tabSpace = 4.0f * charWidth;
        for (size_t i = 0; i < characters.size(); ++i)
        {
            char c = characters[i];

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

            DrawableObject *glyph = new DrawableObject(this, VertexMesh::GetVertexMeshPrototypeByName(std::string(1, c)));
            glyph->SetTexture(Texture::GetTextureByName(font));
            glyph->RotateX(static_cast<float>(M_PI / 2.0));
            glyph->SetPosition(posCursor);
            glyph->SetEmissive(true);
            glyph->SetRenderGraph(1);
            glyph->SetScaleX(charWidth);
            glyphs.push_back(glyph);

            posCursor += vec4(charWidth, 0.0f, 0.0f, 0.0f);
        }
    }
}
