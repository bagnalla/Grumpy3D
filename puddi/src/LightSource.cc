/*************************************************************************************

Program:			CS 4250 Final Project

Author:				Alexander Bagnall
Email:				ab667712@ohio.edu

Description:		Implementation file for the LightSource class.

Date:				December 9, 2015

*************************************************************************************/

#include "LightSource.h"
#include "Puddi.h"
#include "Shader.h"
#include "RenderGraph.h"
#include <cstring>

namespace puddi
{
	// PUBLIC

	void LightSource::Init()
	{
		/*Default.ambient = vec4(1.0, 1.0, 1.0, 1.0);
		Default.diffuse = vec4(1.0, 1.0, 1.0, 1.0);
		Default.specular = vec4(1.0, 1.0, 1.0, 1.0);
		Default.position = vec4(0.0, 0.0, 0.0, 1.0);*/

		for (int i = 0; i < NUM_LIGHT_SOURCES; ++i)
		{
			delete lightSources[i];
			lightSources[i] = new LightSource(i);
			lightSources[i]->ambient = vec4(0.0, 0.0, 0.0, 0.0);
			lightSources[i]->diffuse = vec4(0.0, 0.0, 0.0, 0.0);
			lightSources[i]->specular = vec4(0.0, 0.0, 0.0, 0.0);
			lightSources[i]->position = vec4(0.0, 0.0, 0.0, 0.0);
			lightSources[i]->UpdateMatrix();
		}
	}

	LightSource* LightSource::ObtainLightSource()
	{
		for (int i = 0; i < NUM_LIGHT_SOURCES; ++i)
		{
			if (!lightSources[i]->used)
			{
				lightSources[i]->used = true;
				return lightSources[i];
			}
		}
		return NULL;
	}

	void LightSource::ReleaseLightSource(LightSource *l)
	{
		if (l == NULL)
			return;

		l->used = false;
		l->ambient = vec4(0.0, 0.0, 0.0, 0.0);
		l->diffuse = vec4(0.0, 0.0, 0.0, 0.0);
		l->specular = vec4(0.0, 0.0, 0.0, 0.0);
		l->position = vec4(0.0, 0.0, 0.0, 0.0);
		l->UpdateMatrix();
	}

	void LightSource::UpdateLightSourceMatricesInShaders()
	{
		//glUniformMatrix4fv(engine::LightSourcesLoc, NUM_LIGHT_SOURCES, GL_FALSE, lightSourceMatrices);
		//glUniform4fv(engine::LightSourcesLoc, NUM_LIGHT_SOURCES * 16, lightSourceMatrices);
		mat4 lightSource;
		memcpy(&lightSource[0], lightSourceMatrices, sizeof(lightSourceMatrices));
		Shader::SetLightSource(lightSource);
		//engine::MainRenderGraph->SetLightSourceInAll(lightSource);
	}

	void LightSource::UpdateMatrix()
	{
		//glUniformMatrix4fv(engine::LightSourcesLoc, index, GL_FALSE, mat4(ambient, diffuse, specular, position));
		//lightSourceMatrices[index] = mat4(ambient, diffuse, specular, position);
		mat4 lightSource(ambient, diffuse, specular, position);
		memcpy(&lightSourceMatrices[index * 16], &lightSource, 16 * sizeof(GLfloat));
	}

	// PRIVATE

	LightSource* LightSource::lightSources[NUM_LIGHT_SOURCES];
	GLfloat LightSource::lightSourceMatrices[NUM_LIGHT_SOURCES * 16];

	LightSource::LightSource(int i)
	{
		index = i;
		used = false;
	}
}
