#include "RenderNode.h"
#include "Shader.h"
#include "VertexMesh.h"
#include "TerrainVertexMesh.h"
#include "Texture.h"
#include <iostream>

namespace puddi
{
	// CUBEMAPNODE PUBLIC

	void CubeMapNode::Render() const
	{
		// bind cube map
		glActiveTexture(GL_TEXTURE0 + TEXTURE_CUBE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		// render objects without materials
		Shader::SetTextureBlend(false);
		RenderNode::Render();
		Shader::SetTextureBlend(true);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// CUBEMAPBUMPNODE PUBLIC

	void CubeMapBumpNode::Render() const
	{
		// bind texture
		glActiveTexture(GL_TEXTURE0 + TEXTURE_CUBE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		// bind bump map
		glActiveTexture(GL_TEXTURE0 + TEXTURE_CUBE_BUMP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, bumpMap);

		// render objects without materials
		Shader::SetTextureBlend(false);
		RenderNode::Render();
		Shader::SetTextureBlend(true);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// CUBEMAPEMISSIVENODE PUBLIC

	void CubeMapEmissiveNode::Render() const
	{
		// bind cube map
		glActiveTexture(GL_TEXTURE0 + TEXTURE_CUBE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		// render objects
		RenderNode::Render();
	}

	// TEXTUREBUMPNODE PUBLIC

	void TextureBumpNode::Render() const
	{
		// bind texture
		glActiveTexture(GL_TEXTURE0 + TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		// bind bump map
		glActiveTexture(GL_TEXTURE0 + TEXTURE_2D_BUMP);
		glBindTexture(GL_TEXTURE_2D, bumpmap);

		// render objects without materials
		Shader::SetTextureBlend(false);
		RenderNode::Render();
		Shader::SetTextureBlend(true);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// TEXTURENODE PUBLIC

	void TextureNode::Render() const
	{
		// bind texture
		glActiveTexture(GL_TEXTURE0 + TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		// render objects without materials
		Shader::SetTextureBlend(false);
		RenderNode::Render();
		Shader::SetTextureBlend(true);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// TEXTUREEMISSIVENODE PUBLIC

	void TextureEmissiveNode::Render() const
	{
		// bind texture
		glActiveTexture(GL_TEXTURE0 + TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		// render objects
		RenderNode::Render();
	}

	// MATERIALBUMPNODE PUBLIC

	void MaterialBumpNode::Render() const
	{
		// bind bump map
		glActiveTexture(GL_TEXTURE0 + TEXTURE_2D_BUMP);
		glBindTexture(GL_TEXTURE_2D, bumpmap);

		// render material nodes
		for (auto it = materialNodeMap.begin(); it != materialNodeMap.end(); ++it)
		{
			(*it).second->Render();
		}
	}

	// MATERIALNODE PUBLIC

	void MaterialNode::Render() const
	{
		// send material properties to GPU
		Shader::SetMaterialAmbient(material.ambient);
		Shader::SetMaterialDiffuse(material.diffuse);
		Shader::SetMaterialSpecular(material.specular);
		Shader::SetMaterialShininess(material.shininess);

		// render objects
		RenderNode::Render();
	}

	// EMISSIVENODE PUBLIC

	void EmissiveNode::Render() const
	{
		// send color to GPU
		Shader::SetEmissionColor(emissionColor);

		// render objects
		RenderNode::Render();
	}

	// RENDERNODE PUBLIC

	void RenderNode::Render() const
	{
		// render objects
		for (auto it = meshes.begin(); it != meshes.end(); ++it)
		{
			auto o = *it;
			if (o->GetRenderEnabled())
				o->Draw();
		}
	}
}
