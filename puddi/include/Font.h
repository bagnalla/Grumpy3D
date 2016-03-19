/**
 * @file Font.h
 * @author Alexander Bagnall
 * @brief Provides the ability to render text.
 *
 * Contains a function for loading glyph textures for a font and
 * a convenience function for creating a vector of glyphs from
 * an input string.
 */

#ifndef FONT_H
#define FONT_H

#include <vector>
#include <string>

namespace puddi
{
    class Object;
    class DrawableObject;

	namespace Font
	{
        void Init();

        std::vector<DrawableObject*> CreateGlyphString(Object *rootObject, std::string font, std::string str, float scale = 1.0f);
	};
}

#endif
