#ifndef SOURCECODE_H
#define SOURCECODE_H

#include <vector>
#include <string>
#include "DrawableObject.h"

namespace grumpy
{
    class SourceCode : public puddi::DrawableObject
    {
    public:
        SourceCode(puddi::Object *par, const std::string &path, const std::string &font);

        // parallel vectors
        // characters with no glyph will have a
        // corresponding null entry in the glyphs vector
        std::vector<char> characters;
        std::vector<DrawableObject*> glyphs;
        std::string font;

    private:
        void createGlyphs();
    };
}

#endif
