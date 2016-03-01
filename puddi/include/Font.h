#ifndef FONT_H
#define FONT_H

#include <vector>

namespace puddi
{
    class Rectangle;

	class Font
	{
	public:
        static void Init();

	private:
		static std::vector<Rectangle*> characterRectangles;
	};
}

#endif
