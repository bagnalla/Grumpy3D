#include "Sphere.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"

namespace puddi
{
	// PUBLIC

	void Sphere::Init()
	{
		createPrototypeVertices();
	}

	Sphere::Sphere(Object *par) : DrawableObject(par, VertexMesh::GetVertexMeshPrototypeByName("sphere")) {}

	//void Sphere::Draw() const
	//{
	//	DrawableObject::Draw();
	//	glDrawArrays(GL_TRIANGLE_STRIP, globalVertexOffset, vertexCount);
	//}

	// PRIVATE

	void Sphere::createPrototypeVertices()
	{
		std::vector<vec4> tangents;
		std::vector<vec4> binormals;
		std::vector<vec4> sphere = Geometry::CreateSphere(15.0f, tangents, binormals);
		std::vector<vec4> normals = Geometry::CreateSphereNormals(sphere);

		// sphere texture coordinates
		std::vector<vec2> texCoords;
		for (auto it = normals.begin(); it != normals.end(); ++it)
		{
			auto n = *it;
			float u = asin(n.x) / M_PI + 0.5f;
			float v = asin(n.y) / M_PI + 0.5f;
			texCoords.push_back(vec2(u, v));
		}

		int vertexOffset = Shader::Vertices.size();
		int indexOffset = Shader::VertexIndices.size();
		int indexCount = sphere.size();

		std::vector<uint> indices;
		for (int i = 0; i < indexCount; ++i)
			indices.push_back(vertexOffset + i);

		Shader::Vertices.insert(Shader::Vertices.end(), sphere.begin(), sphere.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
		Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());

		VertexMesh::AddVertexMeshPrototype("sphere", Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)), indexOffset, indexCount, true);
	}
}
