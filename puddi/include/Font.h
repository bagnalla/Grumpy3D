#ifndef FONT_H
#define FONT_H

#include <vector>
#include <string>

namespace puddi
{
    class Rectangle;
    class Object;
    class DrawableObject;

	class Font
	{
	public:
        static void Init();

        static std::vector<DrawableObject*> CreateGlyphString(Object *rootObject, std::string font, std::string str, float scale = 1.0f);

	private:
		static std::vector<Rectangle*> characterRectangles;
	};
}

#endif
