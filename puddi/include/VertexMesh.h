#ifndef VERTEXMESH_H
#define VERTEXMESH_H

//#include "Shader.h"
//#include <glm/vec2.hpp>
//#include <glm/vec4.hpp>
#include "Material.h"
#include <GL/glew.h>
#include <string>
#include <unordered_map>

namespace puddi
{
	struct RenderNode;
	class DrawableObject;

	class VertexMesh
	{
	public:
		// need public default constructor to use it as non-dynamic memory struct
		VertexMesh();

		VertexMesh(DrawableObject *o, const Material& mat, int iOffset, int iCount, bool tStrip);

		virtual ~VertexMesh();

		static void AddVertexMeshPrototype(const std::string &name, const Material& mat, int iOffset, int iCount, bool tStrip);

		static VertexMesh GetVertexMeshPrototypeByName(const std::string &name);

		virtual void Draw() const;

		virtual void UpdateRenderNode();

		void EnableRender();

		void DisableRender();

		void Cull();

		void UnCull();

		DrawableObject* GetOwner() const;
		void SetOwner(DrawableObject *o);

		Material GetMaterial() const;
		void SetMaterial(const Material& mat);

		GLuint GetTexture() const;
		void SetTexture(GLuint tex);

		GLuint GetBumpMap() const;
		void SetBumpMap(GLuint b);

		GLuint GetCubeMap() const;
		void SetCubeMap(GLuint cMap);

		bool GetReflectiveCubeMap() const;
		void SetReflectiveCubeMap(bool b);

		bool GetBumpMapEnabled() const;
		void SetBumpMapEnabled(bool b);

		bool GetEmissive() const;
		void SetEmissive(bool b);

		vec4 GetEmissionColor() const;
		void SetEmissionColor(const vec4& eColor);

		bool GetRenderEnabled() const;
		void SetRenderEnabled(bool b);

		size_t GetRenderGraphIndex() const;
		void SetRenderGraphIndex(size_t i);

	private:
		static std::unordered_map<std::string, VertexMesh> vertexMeshPrototypeMap;

		DrawableObject *owner;
		Material material;
		GLuint texture;
		GLuint cubeMap;
		bool reflectiveCubeMap;
		bool bumpMapEnabled;
		GLuint bumpmap;
		bool emissive;
		vec4 emissionColor;
		RenderNode *renderNode;
		bool renderEnabled;

		bool triangleStrip;

		void init();

	protected:
		uint indexOffset;
		uint indexCount;
		size_t renderGraphIndex;
	};
}

#endif
