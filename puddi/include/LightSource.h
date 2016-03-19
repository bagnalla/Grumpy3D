/**
 * @file LightSource.h
 * @author Alexander Bagnall
 * @brief Provides functions for manipulating the world's light source(s)
 *
 * Currently, none of the shader programs support multiple light
 * sources. It isn't terribly difficult to add but I don't consider
 * it to be important.
 */

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#define NUM_LIGHT_SOURCES 1

#include <GL/glew.h>
#include "GlmHeaders.h"

namespace puddi
{
	class LightSource
	{
	public:
		static void Init();

		static LightSource* ObtainLightSource();

		static void ReleaseLightSource(LightSource *l);

		static void UpdateLightSourceMatricesInShaders();

		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 position;

		void UpdateMatrix();

	private:
		static LightSource *lightSources[NUM_LIGHT_SOURCES];
		static GLfloat lightSourceMatrices[NUM_LIGHT_SOURCES * 16];

		int index;
		bool used;

		LightSource(int i);
	};
}

#endif
