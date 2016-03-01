#ifndef CUBE_H
#define CUBE_H

#include "DrawableObject.h"

namespace puddi
{
	class Cube : public DrawableObject
	{
	public:
		static void Init();

		Cube(Object *par);

		bool ContainsPoint(const vec4& p) const;

	private:

		static void createPrototypeVertices();
	};
}

#endif
