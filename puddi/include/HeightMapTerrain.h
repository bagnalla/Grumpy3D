#ifndef HEIGHTMAPTERRAIN_H
#define HEIGHTMAPTERRAIN_H

namespace puddi
{
	class TerrainVertexMesh;

	class HeightMapTerrain
	{
	public:
		static TerrainVertexMesh CreateTerrainMeshFromFile(const char *fileName, float texWidth, float texHeight, float scaleZ);

	private:
	};
}

#endif
