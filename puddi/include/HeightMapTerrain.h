/**
 * @file HeightMapTerrain.h
 * @author Alexander Bagnall
 * @brief Provides a function for creating a TerrainVertexMesh from a
 * height map file.
 */

#ifndef HEIGHTMAPTERRAIN_H
#define HEIGHTMAPTERRAIN_H

namespace puddi
{
	class TerrainVertexMesh;

	namespace HeightMapTerrain
	{
		TerrainVertexMesh CreateTerrainMeshFromFile(const char *fileName, float texWidth, float texHeight, float scaleZ);
	};
}

#endif
