#ifndef SPHERE_H
#define SPHERE_H

#include "DrawableObject.h"

namespace puddi
{
	class Sphere : public DrawableObject
	{
	public:
		static void Init();

		Sphere(Object *par);

		//void Draw() const;

	private:

		static void createPrototypeVertices();
	};
}

#endif
