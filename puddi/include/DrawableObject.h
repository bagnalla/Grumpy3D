#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "Object.h"
#include "VertexMesh.h"

namespace puddi
{
	class VertexMesh;
	class TerrainVertexMesh;
	struct SchematicNode;

	class DrawableObject : public Object
	{
	public:
		DrawableObject(Object *par);

		DrawableObject(Object *par, const VertexMesh &mesh);

		DrawableObject(Object *par, TerrainVertexMesh *mesh);

		DrawableObject(Object *par, SchematicNode *schematic);

		~DrawableObject();

		//virtual void Draw() const;

		void SendTransformToGPU();

		void AddVertexMesh(VertexMesh *vertexMesh);

		void SetEmissive(bool e);

		virtual void SetEmissionColor(const vec4& color);

		//void SetBlend(bool b);

		virtual void SetMaterial(const Material& m);

		void SetTexture(GLuint tex);

		void SetBumpMap(GLuint bump);

		void SetCubeMap (GLuint cm);

		void EnableRender(bool propagate = true);

		void DisableRender(bool propagate = true);

		void Cull();

		void UnCull();

		bool GetRenderEnabled() const;

		void SetBumpMapEnabled(bool b);

		void SetReflectiveCubeMap(bool b);

		void DisableShadowCasting();

		void SetRenderGraph(size_t index);

	private:
		bool renderEnabled;

	protected:
		std::vector<VertexMesh*> vertexMeshes;

		void init();

		void updateRenderNodes();
	};
}

#endif
