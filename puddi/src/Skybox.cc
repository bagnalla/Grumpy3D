#include "Skybox.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"

namespace puddi
{
	// PUBLIC

	void Skybox::Init()
	{
		createPrototypeVertices();
	}

	Skybox::Skybox(Object *par) : DrawableObject(par, VertexMesh::GetVertexMeshPrototypeByName("skybox"))
	{
		RotateX(-M_PI / 2.0f);
		RotateZ(M_PI);
		AddIgnoreParentModelFlag(IGNORE_PARENT_ROTATIONX | IGNORE_PARENT_ROTATIONY | IGNORE_PARENT_ROTATIONZ);
	}

//	void Skybox::Draw() const
//	{
//		DrawableObject::Draw();
//		glDrawArrays(GL_TRIANGLES, indexOffset, indexCount);
//	}

	// PRIVATE

	void Skybox::createPrototypeVertices()
	{
		std::vector<vec4> cube = Geometry::CreateCube(1.0);
		std::vector<vec4> normals = Geometry::CreateCubeNormals();
		// negate the normals
		Util::Apply(normals, [](const vec4& v) -> vec4
		{
			return -v;
		});
		std::vector<vec4> tangents = Geometry::CreateCubeTangents();
		std::vector<vec4> binormals = Geometry::CreateCubeBiNormals();
		std::vector<vec2> texCoords = Geometry::CreateSkyboxTextureCoordinates();

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

		VertexMesh::AddVertexMeshPrototype("skybox", Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)), indexOffset, indexCount, false);
	}
}
