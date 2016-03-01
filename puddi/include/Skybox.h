#ifndef SKYBOX_H
#define SKYBOX_H

#include "DrawableObject.h"

namespace puddi
{
	class Skybox : public DrawableObject
	{
	public:
		static void Init();

		Skybox(Object *par);

		//void Draw() const;

	private:

		static void createPrototypeVertices();
	};
}

#endif
