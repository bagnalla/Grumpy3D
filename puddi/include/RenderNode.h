#ifndef RENDERNODE_H
#define RENDERNODE_H

#include "Material.h"
#include <glm/vec4.hpp>
#include <GL/glew.h>
#include <unordered_map>

namespace puddi
{
	class VertexMesh;
	class TerrainVertexMesh;

	struct RenderNode
	{
		std::vector<VertexMesh*> meshes;

		virtual void Render() const;
	};

	struct EmissiveNode : public RenderNode
	{
		vec4 emissionColor;

		EmissiveNode(const vec4& emissionCol) { emissionColor = emissionCol; }

		void Render() const;
	};

	struct MaterialNode : public RenderNode
	{
		Material material;

		MaterialNode(const Material& mat) { material = mat; }

		void Render() const;
	};

	struct MaterialBumpNode : public RenderNode
	{
		GLuint bumpmap;
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		MaterialBumpNode(GLuint bump) { bumpmap = bump; }

		void Render() const;
	};

	struct TextureBumpNode : public RenderNode
	{
		GLuint texture;
		GLuint bumpmap;
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		TextureBumpNode(GLuint tex, GLuint bump) { texture = tex; bumpmap = bump; }

		void Render() const;
	};

	struct TextureNode : public RenderNode
	{
		GLuint texture;
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		TextureNode(GLuint tex) { texture = tex; }

		void Render() const;
	};

	struct TextureEmissiveNode : public RenderNode
	{
		GLuint texture;

		TextureEmissiveNode(GLuint tex) { texture = tex; }

		void Render() const;
	};

	struct CubeMapNode : public RenderNode
	{
		GLuint cubeMap;

		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		CubeMapNode(GLuint cubeM) { cubeMap = cubeM; }

		void Render() const;
	};

	struct CubeMapBumpNode : public RenderNode
	{
		GLuint cubeMap;
		GLuint bumpMap;
		std::unordered_map<Material, MaterialNode*> materialNodeMap;

		CubeMapBumpNode(GLuint cubeM, GLuint bump) { cubeMap = cubeM; bumpMap = bump; }

		void Render() const;
	};

	struct CubeMapEmissiveNode : public RenderNode
	{
		GLuint cubeMap;

		CubeMapEmissiveNode(GLuint cubeM) { cubeMap = cubeM; }

		void Render() const;
	};
}

#endif
