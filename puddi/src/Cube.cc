#include "Cube.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"
#include "VertexMesh.h"
#include "Material.h"

namespace puddi
{
	// PUBLIC

	void Cube::Init()
	{
		createPrototypeVertices();
	}

	Cube::Cube(Object *par) : DrawableObject(par, VertexMesh::GetVertexMeshPrototypeByName("cube")) {}

	//void Cube::Draw() const
	//{
	//	DrawableObject::Draw();
	//	glDrawArrays(GL_TRIANGLES, globalVertexOffset, vertexCount);
	//}

	bool Cube::ContainsPoint(const vec4& p) const
	{
		vec4 tp = Util::RotateX(-theta.x) * Util::RotateY(-theta.y) * Util::RotateZ(-theta.z)
			* translate(-vec3(position.x, position.y, position.z))
			* p;

		return (tp.x >= -scale.x / 2.0 && tp.x <= scale.x / 2.0
			&& tp.y >= -scale.y / 2.0 && tp.y <= scale.y / 2.0
			&& tp.z >= -scale.z / 2.0 && tp.z <= scale.z / 2.0);
	}

	// PRIVATE

	void Cube::createPrototypeVertices()
	{
		std::vector<vec4> cube = Geometry::CreateCube(1.0);
		std::vector<vec4> normals = Geometry::CreateCubeNormals();
		std::vector<vec4> tangents = Geometry::CreateCubeTangents();
		std::vector<vec4> binormals = Geometry::CreateCubeBiNormals();
		std::vector<vec2> texCoords = Geometry::CreateCubeTextureCoordinates();

		int vertexOffset = Shader::Vertices.size();
		int indexOffset = Shader::VertexIndices.size();
		int indexCount = cube.size();

		std::vector<uint> indices;
		for (int i = 0; i < indexCount; ++i)
			indices.push_back(vertexOffset + i);

		Shader::Vertices.insert(Shader::Vertices.end(), cube.begin(), cube.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
		Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());

		VertexMesh::AddVertexMeshPrototype("cube", Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)), indexOffset, indexCount, false);
	}
}
