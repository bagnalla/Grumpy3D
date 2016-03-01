#include "VertexMesh.h"
#include "DrawableObject.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderGraph.h"
#include "Puddi.h"
#include "Shadow.h"
#include <algorithm>
#include <iostream>

namespace puddi
{
	// PUBLIC

	VertexMesh::VertexMesh()
	{
		init();
	}

	VertexMesh::VertexMesh(DrawableObject *o, const Material& mat, int iOffset, int iCount, bool tStrip)
	{
		init();

		owner = o;
		material = mat;
		indexOffset = iOffset;
		indexCount = iCount;
		triangleStrip = tStrip;
		texture = 0;
		renderGraphIndex = 0;
	}

	VertexMesh::~VertexMesh()
	{
        //std::cout << "in VertexMesh destructor\n";
		if (renderNode != NULL)
		{
			auto it = std::find(renderNode->meshes.begin(), renderNode->meshes.end(), this);
            if (it != renderNode->meshes.end())
                renderNode->meshes.erase(std::find(renderNode->meshes.begin(), renderNode->meshes.end(), this));
            else
                std::cerr << "in VerteshMesh destructor: attempted to remove self from rendernode that didnt know about me. this should never happen\n";
		}

		Shadow::RemoveFromDepthRenderList(this);
	}

	void VertexMesh::AddVertexMeshPrototype(const std::string &name, const Material& mat, int iOffset, int iCount, bool tStrip)
	{
		auto it = vertexMeshPrototypeMap.find(name);
		if (it == vertexMeshPrototypeMap.end())
			vertexMeshPrototypeMap.emplace(name, VertexMesh(NULL, mat, iOffset, iCount, tStrip));
		else
			std::cerr << "vertex mesh already exists with name: " << name << std::endl;
	}

	VertexMesh VertexMesh::GetVertexMeshPrototypeByName(const std::string &name)
	{
		auto it = vertexMeshPrototypeMap.find(name);
		if (it != vertexMeshPrototypeMap.end())
			return vertexMeshPrototypeMap[name];
		else
		{
			std::cerr << "vertex mesh doesn't exist with name: " << name << std::endl;
			return VertexMesh(NULL, Material::None(), 0, 0, 0);
		}
	}

	void VertexMesh::Draw() const
	{
		owner->SendTransformToGPU();

		if (cubeMap != 0)
			Shader::SetReflectiveCubeMap(reflectiveCubeMap);

		// hacky way of doing triangle strip instead of triangles when the bool is set
		// because GL_TRIANGLES is 4 and GL_TRIANGLE_STRIP is 5
		//glDrawArrays(GL_TRIANGLES + triangleStrip, globalVertexOffset, vertexCount);
		glDrawElements(GL_TRIANGLES + triangleStrip, indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(indexOffset * sizeof(uint)));
	}

	void VertexMesh::UpdateRenderNode()
	{
		if (renderNode != NULL)
		{
            auto it = std::find(renderNode->meshes.begin(), renderNode->meshes.end(), this);
            if (it != renderNode->meshes.end())
                renderNode->meshes.erase(std::find(renderNode->meshes.begin(), renderNode->meshes.end(), this));
            else
                std::cerr << "in VerteshMesh::UpdateRenderNode(): attempted to remove self from rendernode that didnt know about me. this should never happen\n";
        }

        renderNode = Puddi::GetRenderGraph(renderGraphIndex)->AddVertexMesh(this);

		Shadow::AddToDepthRenderList(this);
	}

	void VertexMesh::EnableRender()
	{
		renderEnabled = true;
	}

	void VertexMesh::DisableRender()
	{
		renderEnabled = false;
	}

	void VertexMesh::Cull()
	{
		if (renderNode != NULL)
			renderNode->meshes.erase(std::find(renderNode->meshes.begin(), renderNode->meshes.end(), this));
		renderNode = NULL;

		Shadow::RemoveFromDepthRenderList(this);
	}

	void VertexMesh::UnCull()
	{
		Shadow::AddToDepthRenderList(this);
		UpdateRenderNode();
	}

	DrawableObject* VertexMesh::GetOwner() const
	{
		return owner;
	}
	void VertexMesh::SetOwner(DrawableObject *o)
	{
		owner = o;
	}

	Material VertexMesh::GetMaterial() const
	{
		return material;
	}
	void VertexMesh::SetMaterial(const Material& mat)
	{
		material = mat;
	}

	GLuint VertexMesh::GetTexture() const
	{
		return texture;
	}
	void VertexMesh::SetTexture(GLuint tex)
	{
		texture = tex;
	}

	GLuint VertexMesh::GetBumpMap() const
	{
		return bumpmap;
	}
	void VertexMesh::SetBumpMap(GLuint b)
	{
		bumpmap = b;
	}

	GLuint VertexMesh::GetCubeMap() const
	{
		return cubeMap;
	}
	void VertexMesh::SetCubeMap(GLuint cMap)
	{
		cubeMap = cMap;
	}

	bool VertexMesh::GetReflectiveCubeMap() const
	{
		return reflectiveCubeMap;
	}
	void VertexMesh::SetReflectiveCubeMap(bool b)
	{
		reflectiveCubeMap = b;
	}

	bool VertexMesh::GetBumpMapEnabled() const
	{
		return bumpMapEnabled;
	}
	void VertexMesh::SetBumpMapEnabled(bool b)
	{
		bumpMapEnabled = b;
	}

	bool VertexMesh::GetEmissive() const
	{
		return emissive;
	}
	void VertexMesh::SetEmissive(bool b)
	{
		emissive = b;
	}

	vec4 VertexMesh::GetEmissionColor() const
	{
		return emissionColor;
	}
	void VertexMesh::SetEmissionColor(const vec4& eColor)
	{
		emissionColor = eColor;
	}

	bool VertexMesh::GetRenderEnabled() const
	{
		return renderEnabled;
	}
	void VertexMesh::SetRenderEnabled(bool b)
	{
		renderEnabled = b;
	}

	bool VertexMesh::GetRenderGraphIndex() const
	{
		return renderGraphIndex;
	}
	void VertexMesh::SetRenderGraphIndex(size_t i)
	{
		renderGraphIndex = i;
	}

	// PRIVATE

	std::unordered_map<std::string, VertexMesh> VertexMesh::vertexMeshPrototypeMap;

	void VertexMesh::init()
	{
		owner = NULL;
		material = Material::None();
		texture = 0;
		bumpmap = 0;
		cubeMap = 0;
		reflectiveCubeMap = false;
		bumpMapEnabled = true;
		emissive = false;
		emissionColor = vec4();
		renderNode = NULL;
		renderEnabled = true;

		indexOffset = 0;
		indexCount = 0;
		triangleStrip = false;
	}
}
