#ifndef TERRAINVERTEXMESH_H
#define TERRAINVERTEXMESH_H

#include "VertexMesh.h"

namespace puddi
{
	class TerrainVertexMesh : public VertexMesh
	{
	public:
		TerrainVertexMesh(const Material& mat, int iOffset, int iCount);

		~TerrainVertexMesh();

		void DrawWithoutBumpMap() const;

		void DrawWithBumpMap() const;

		void UpdateRenderNode();

		GLuint GetTexture1() const;
		GLuint GetTexture2() const;
		GLuint GetTexture3() const;
		GLuint GetTexture4() const;
		vec4 GetTextureHeights() const;
		float GetGroundCoordZ() const;
		float GetScaleZ() const;
		float GetMaxHeight() const;
		int GetSizeX() const;
		int GetSizeY() const;

		void SetTexture1(GLuint tex1);
		void SetTexture2(GLuint tex2);
		void SetTexture3(GLuint tex3);
		void SetTexture4(GLuint tex4);
		void SetTextureHeights(const vec4& texHeights);
		void SetGroundCoordZ(float z);
		void SetScaleZ(float z);
		void SetMaxHeight(float h);
		void SetSizeX(int x);
		void SetSizeY(int y);

	private:
		GLuint texture1;
		GLuint texture2;
		GLuint texture3;
		GLuint texture4;
		vec4 textureHeights;
		float groundCoordZ;
		float scaleZ;
		float maxHeight;

		int sizeX;  // this * scale.x = x size of mesh
		int sizeY; // this * scale.y = y size of mesh
	};
}

#endif
