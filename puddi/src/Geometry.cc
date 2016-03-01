#include "Geometry.h"
#include "Util.h"
#include "CRSpline.h"

namespace puddi
{
	// PUBLIC

	std::vector<vec4> Geometry::CreateCatmullRomCurve(const vec4& p0, const vec4& p1, const vec4& p2, const vec4& p3, float curviness)
	{
		float weightIncrement = 1.0 / curviness;

		std::vector<vec4> vectors;
		for (float s = weightIncrement; s < 1.0; s += weightIncrement)
		{
			vectors.push_back(CRSpline::Eq(s, p0, p1, p2, p3));
		}

		vectors.push_back(p2);

		return vectors;
	}

	// need at least 3 vertices
	std::vector<vec4> Geometry::CreateBlob(float curviness, const vec4 verts[], int numOfVerts)
	{
		std::vector<vec4> vertices;

		std::vector<vec4> curve = CreateCatmullRomCurve(verts[numOfVerts - 1], verts[0], verts[1], verts[2], curviness);
		vertices.insert(vertices.end(), curve.begin(), curve.end());

		for (int i = 1; i < numOfVerts - 2; i++)
		{
			curve = CreateCatmullRomCurve(verts[i - 1], verts[i], verts[i + 1], verts[i + 2], curviness);
			vertices.insert(vertices.end(), curve.begin(), curve.end());
		}

		curve = CreateCatmullRomCurve(verts[numOfVerts - 3], verts[numOfVerts - 2], verts[numOfVerts - 1], verts[0], curviness);
		vertices.insert(vertices.end(), curve.begin(), curve.end());

		curve = CreateCatmullRomCurve(verts[numOfVerts - 2], verts[numOfVerts - 1], verts[0], verts[1], curviness);
		vertices.insert(vertices.end(), curve.begin(), curve.end());

		vertices.push_back(vertices[0]);

		return vertices;
	}

	std::vector<vec4> Geometry::CreateCircle(int numberOfVertices)
	{
		std::vector<vec4> vertices;
		float increment = (2 * M_PI) / (numberOfVertices - 1);
		for (float theta = 0.0f; theta <= 2 * M_PI; theta += increment)
			vertices.push_back(vec4(cos(theta), sin(theta), 0, 1));
		vertices.push_back(vec4(1.0, 0, 0, 1));
		return vertices;
	}

	std::vector<vec4> Geometry::CreateCircle(int numberOfVertices, std::vector<vec4>& tangents)
	{
		tangents.clear();
		vec4 tangent = vec4(0.0, 1.0, 0.0, 0.0);
		std::vector<vec4> vertices;
		float increment = (2 * M_PI) / (numberOfVertices - 1);
		for (float theta = 0.0f; theta <= 2 * M_PI; theta += increment)
		{
			vertices.push_back(vec4(cos(theta), sin(theta), 0, 1));
			tangents.push_back(vec4(Util::RotateZ(theta) * tangent));
		}
		vertices.push_back(vec4(1.0, 0, 0, 1));
		tangents.push_back(tangent);
		return vertices;
	}

	std::vector<vec4> Geometry::CreateSphere(int definition)
	{
		float deltaTheta = M_PI / definition;
		float theta = 0;
		std::vector<vec4> sphere;

		std::vector<vec4> originalRing = Geometry::CreateCircle(definition * 2);
		std::vector<vec4> ring1 = originalRing;
		std::vector<vec4> ring2;

		for (int i = 0; i < definition + 1; i++)
		{
			theta += deltaTheta;
			ring2 = Util::TransformVertices(originalRing, Util::RotateY(theta));

			for (int j = 0; j < ring1.size(); j++)
			{
				sphere.push_back(ring1[j]);
				sphere.push_back(ring2[j]);
			}

			sphere.push_back(ring1[0]);
			sphere.push_back(ring2[0]);

			ring1 = ring2;
			ring2 = Util::TransformVertices(originalRing, Util::RotateY(theta));
		}

		/*	for (int j = 0; j < ring1.size(); j++)
		{
		sphere.push_back(originalRing[j]);
		sphere.push_back(ring2[j]);
		}
		sphere.push_back(originalRing[0]);
		sphere.push_back(ring2[0]);*/

		return sphere;
	}

	std::vector<vec4> Geometry::CreateSphere(int definition, std::vector<vec4>& tangents, std::vector<vec4>& binormals)
	{
		tangents.clear();
		binormals.clear();

		float deltaTheta = M_PI / definition;
		float theta = 0;
		std::vector<vec4> sphere;

		std::vector<vec4> originalTangents;
		std::vector<vec4> originalRing = Geometry::CreateCircle(definition * 2, originalTangents);
		std::vector<vec4> ring1 = originalRing;
		std::vector<vec4> ring2;
		std::vector<vec4> ring1Tangents = originalTangents;
		std::vector<vec4> ring2Tangents;

		for (int i = 0; i < definition + 1; i++)
		{
			theta += deltaTheta;
			ring2 = Util::TransformVertices(originalRing, Util::RotateY(theta));
			ring2Tangents = Util::TransformVertices(originalTangents, Util::RotateY(theta));

			for (int j = 0; j < ring1.size(); j++)
			{
				sphere.push_back(ring1[j]);
				sphere.push_back(ring2[j]);
				tangents.push_back(ring1Tangents[j]);
				tangents.push_back(ring2Tangents[j]);
				binormals.push_back(vec4(normalize(cross(vec3(ring1[j]), vec3(ring1Tangents[j]))), 0.0f));
				binormals.push_back(vec4(normalize(cross(vec3(ring2[j]), vec3(ring2Tangents[j]))), 0.0f));
			}

			sphere.push_back(ring1[0]);
			sphere.push_back(ring2[0]);
			tangents.push_back(ring1Tangents[0]);
			tangents.push_back(ring2Tangents[0]);
			binormals.push_back(vec4(normalize(cross(vec3(ring1[0]), vec3(ring1Tangents[0]))), 0.0f));
			binormals.push_back(vec4(normalize(cross(vec3(ring2[0]), vec3(ring2Tangents[0]))), 0.0f));

			ring1 = ring2;
			ring1Tangents = ring2Tangents;
			ring2 = Util::TransformVertices(originalRing, Util::RotateY(theta));
			ring2Tangents = Util::TransformVertices(originalTangents, Util::RotateY(theta));
		}

		return sphere;
	}

	// used for CreateSphere2
	vec4 tetraPoints[4] = { vec4(0.0, 0.0, 1.0, 1.0),
		vec4(0.0, 0.942809, -0.333333, 1.0),
		vec4(-0.816497, -0.471405, -0.333333, 1.0),
		vec4(0.816497, -0.471405, -0.333333, 1.0) };
	void triangle(std::vector<vec4>& spherePoints, vec4 a, vec4 b, vec4 c)
	{
		spherePoints.push_back(vec4(a.x, a.y, a.z, 1.0));
		spherePoints.push_back(vec4(b.x, b.y, b.z, 1.0));
		spherePoints.push_back(vec4(c.x, c.y, c.z, 1.0));
	}
	void divide_triangle(std::vector<vec4>& spherePoints, vec4 a, vec4 b, vec4 c, int n)
	{
		vec4 v1, v2, v3;
		if (n > 0)
		{
			v1 = normalize(vec4(vec3(a + b), 0.0));
			v2 = normalize(vec4(vec3(a + c), 0.0));
			v3 = normalize(vec4(vec3(b + c), 0.0));
			divide_triangle(spherePoints, a, v2, v1, n - 1);
			divide_triangle(spherePoints, c, v3, v2, n - 1);
			divide_triangle(spherePoints, b, v1, v3, n - 1);
			divide_triangle(spherePoints, v1, v2, v3, n - 1);
		}
		else triangle(spherePoints, a, b, c);
	}
	void tetrahedron(std::vector<vec4>& spherePoints, int n)
	{
		divide_triangle(spherePoints, tetraPoints[0], tetraPoints[1], tetraPoints[2], n);
		divide_triangle(spherePoints, tetraPoints[3], tetraPoints[2], tetraPoints[1], n);
		divide_triangle(spherePoints, tetraPoints[0], tetraPoints[3], tetraPoints[1], n);
		divide_triangle(spherePoints, tetraPoints[0], tetraPoints[2], tetraPoints[3], n);
	}

	std::vector<vec4> Geometry::CreateSphere2(int n)
	{
		std::vector<vec4> spherePoints;
		tetrahedron(spherePoints, n);
		return spherePoints;
	}

	std::vector<vec4> Geometry::CreateCube(float size)
	{
		// taken from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
		vec4 verts[] = {

			// FRONT
			//vec4(-size / 2.0, -size / 2.0, -size / 2.0, 1.0),
			//vec4(-size / 2.0, -size / 2.0, size / 2.0, 1.0),	// 5
			//vec4(size / 2.0, -size / 2.0, size / 2.0, 1.0),

			//vec4(size / 2.0, -size / 2.0, size / 2.0, 1.0),
			//vec4(size / 2.0, -size / 2.0, -size / 2.0, 1.0),	// 2
			//vec4(-size / 2.0, -size / 2.0, -size / 2.0, 1.0),

			// FRONT
			vec4(size / 2.0, -size / 2.0, -size / 2.0, 1.0),
			vec4(size / 2.0, -size / 2.0, size / 2.0, 1.0),
			vec4(-size / 2.0, -size / 2.0, size / 2.0, 1.0),	// 2

			vec4(-size / 2.0, -size / 2.0, size / 2.0, 1.0),
			vec4(-size / 2.0, -size / 2.0, -size / 2.0, 1.0),
			vec4(size / 2.0, -size / 2.0, -size / 2.0, 1.0), // 5

			// BACK
			vec4(-size / 2.0, size / 2.0, -size / 2.0, 1.0),
			vec4(-size / 2.0, size / 2.0, size / 2.0, 1.0),		// 10
			vec4(size / 2.0, size / 2.0, size / 2.0, 1.0),

			vec4(size / 2.0, size / 2.0, size / 2.0, 1.0),
			vec4(size / 2.0, size / 2.0, -size / 2.0, 1.0),		// 9
			vec4(-size / 2.0, size / 2.0, -size / 2.0, 1.0),

			// LEFT
			vec4(-size / 2.0, -size / 2.0, -size / 2.0, 1.0),
			vec4(-size / 2.0, -size / 2.0, size / 2.0, 1.0),	// 0
			vec4(-size / 2.0, size / 2.0, size / 2.0, 1.0),

			vec4(-size / 2.0, size / 2.0, size / 2.0, 1.0),
			vec4(-size / 2.0, size / 2.0, -size / 2.0, 1.0),	// 4
			vec4(-size / 2.0, -size / 2.0, -size / 2.0, 1.0),

			// TOP
			vec4(-size / 2.0, -size / 2.0, -size / 2.0, 1.0),
			vec4(-size / 2.0, size / 2.0, -size / 2.0, 1.0),	// 1
			vec4(size / 2.0, size / 2.0, -size / 2.0, 1.0),

			vec4(size / 2.0, size / 2.0, -size / 2.0, 1.0),
			vec4(size / 2.0, -size / 2.0, -size / 2.0, 1.0),	// 3
			vec4(-size / 2.0, -size / 2.0, -size / 2.0, 1.0),

			// RIGHT
			//vec4(size / 2.0, -size / 2.0, -size / 2.0, 1.0),
			//vec4(size / 2.0, -size / 2.0, size / 2.0, 1.0),		// 7
			//vec4(size / 2.0, size / 2.0, size / 2.0, 1.0),

			//vec4(size / 2.0, size / 2.0, size / 2.0, 1.0),
			//vec4(size / 2.0, size / 2.0, -size / 2.0, 1.0),		// 8
			//vec4(size / 2.0, -size / 2.0, -size / 2.0, 1.0),

			vec4(size / 2.0, size / 2.0, -size / 2.0, 1.0),
			vec4(size / 2.0, size / 2.0, size / 2.0, 1.0),		// 7
			vec4(size / 2.0, -size / 2.0, size / 2.0, 1.0),

			vec4(size / 2.0, -size / 2.0, size / 2.0, 1.0),
			vec4(size / 2.0, -size / 2.0, -size / 2.0, 1.0),		// 8
			vec4(size / 2.0, size / 2.0, -size / 2.0, 1.0),

			// BOTTOM
			vec4(-size / 2.0, size / 2.0, size / 2.0, 1.0),
			vec4(-size / 2.0, -size / 2.0, size / 2.0, 1.0),	// 6
			vec4(size / 2.0, -size / 2.0, size / 2.0, 1.0),

			vec4(size / 2.0, -size / 2.0, size / 2.0, 1.0),
			vec4(size / 2.0, size / 2.0, size / 2.0, 1.0),		// 11
			vec4(-size / 2.0, size / 2.0, size / 2.0, 1.0)
		};

		std::vector<vec4> v;
		v.assign(verts, verts + 36);
		return v;
	}

	std::vector<vec4> Geometry::CreateCubeNormals()
	{
		vec4 verts[] = {

			vec4(0.0, -1.0, 0.0, 0.0),
			vec4(0.0, -1.0, 0.0, 0.0),	// 5
			vec4(0.0, -1.0, 0.0, 0.0),

			vec4(0.0, -1.0, 0.0, 0.0),
			vec4(0.0, -1.0, 0.0, 0.0),	// 2
			vec4(0.0, -1.0, 0.0, 0.0),

			vec4(0.0, 1.0, 0.0, 0.0),
			vec4(0.0, 1.0, 0.0, 0.0),	// 10
			vec4(0.0, 1.0, 0.0, 0.0),

			vec4(0.0, 1.0, 0.0, 0.0),
			vec4(0.0, 1.0, 0.0, 0.0),	// 9
			vec4(0.0, 1.0, 0.0, 0.0),

			vec4(-1.0, 0.0, 0.0, 0.0),
			vec4(-1.0, 0.0, 0.0, 0.0),	// 0
			vec4(-1.0, 0.0, 0.0, 0.0),

			vec4(-1.0, 0.0, 0.0, 0.0),
			vec4(-1.0, 0.0, 0.0, 0.0),	// 4
			vec4(-1.0, 0.0, 0.0, 0.0),

			vec4(0.0, 0.0, -1.0, 0.0),
			vec4(0.0, 0.0, -1.0, 0.0),	// 1
			vec4(0.0, 0.0, -1.0, 0.0),

			vec4(0.0, 0.0, -1.0, 0.0),
			vec4(0.0, 0.0, -1.0, 0.0),	// 3
			vec4(0.0, 0.0, -1.0, 0.0),

			vec4(1.0, 0.0, 0.0, 0.0),
			vec4(1.0, 0.0, 0.0, 0.0),	// 7
			vec4(1.0, 0.0, 0.0, 0.0),

			vec4(1.0, 0.0, 0.0, 0.0),
			vec4(1.0, 0.0, 0.0, 0.0),	// 8
			vec4(1.0, 0.0, 0.0, 0.0),

			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 6
			vec4(0.0, 0.0, 1.0, 0.0),

			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 11
			vec4(0.0, 0.0, 1.0, 0.0)
		};

		std::vector<vec4> v;
		v.assign(verts, verts + 36);
		return v;
	}

	std::vector<vec4> Geometry::CreateCubeTangents()
	{
		vec4 verts[] = {

			vec4(1.0, 0.0, 0.0, 0.0),
			vec4(1.0, 0.0, 0.0, 0.0),	// 5
			vec4(1.0, 0.0, 0.0, 0.0),

			vec4(1.0, 0.0, 0.0, 0.0),
			vec4(1.0, 0.0, 0.0, 0.0),	// 2
			vec4(1.0, 0.0, 0.0, 0.0),
			
			vec4(-1.0, 0.0, 0.0, 0.0),
			vec4(-1.0, 0.0, 0.0, 0.0),	// 10
			vec4(-1.0, 0.0, 0.0, 0.0),

			vec4(-1.0, 0.0, 0.0, 0.0),
			vec4(-1.0, 0.0, 0.0, 0.0),	// 9
			vec4(-1.0, 0.0, 0.0, 0.0),
			
			vec4(0.0, -1.0, 0.0, 0.0),
			vec4(0.0, -1.0, 0.0, 0.0),	// 0
			vec4(0.0, -1.0, 0.0, 0.0),

			vec4(0.0, -1.0, 0.0, 0.0),
			vec4(0.0, -1.0, 0.0, 0.0),	// 4
			vec4(0.0, -1.0, 0.0, 0.0),
			
			vec4(-1.0, 0.0, 0.0, 0.0),
			vec4(-1.0, 0.0, 0.0, 0.0),	// 1
			vec4(-1.0, 0.0, 0.0, 0.0),

			vec4(-1.0, 0.0, 0.0, 0.0),
			vec4(-1.0, 0.0, 0.0, 0.0),	// 3
			vec4(-1.0, 0.0, 0.0, 0.0),
			
			vec4(0.0, 1.0, 0.0, 0.0),
			vec4(0.0, 1.0, 0.0, 0.0),	// 7
			vec4(0.0, 1.0, 0.0, 0.0),

			vec4(0.0, 1.0, 0.0, 0.0),
			vec4(0.0, 1.0, 0.0, 0.0),	// 8
			vec4(0.0, 1.0, 0.0, 0.0),
			
			vec4(-1.0, 0.0, 0.0, 0.0),
			vec4(-1.0, 0.0, 0.0, 0.0),	// 6
			vec4(-1.0, 0.0, 0.0, 0.0),

			vec4(-1.0, 0.0, 0.0, 0.0),
			vec4(-1.0, 0.0, 0.0, 0.0),	// 11
			vec4(-1.0, 0.0, 0.0, 0.0)
		};

		std::vector<vec4> v;
		v.assign(verts, verts + 36);
		return v;
	}

	std::vector<vec4> Geometry::CreateCubeBiNormals()
	{
		vec4 verts[] = {

			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 5
			vec4(0.0, 0.0, 1.0, 0.0),

			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 2
			vec4(0.0, 0.0, 1.0, 0.0),
			
			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 10
			vec4(0.0, 0.0, 1.0, 0.0),

			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 9
			vec4(0.0, 0.0, 1.0, 0.0),
			
			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 0
			vec4(0.0, 0.0, 1.0, 0.0),

			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 4
			vec4(0.0, 0.0, 1.0, 0.0),
			
			vec4(0.0, 1.0, 0.0, 0.0),
			vec4(0.0, 1.0, 0.0, 0.0),	// 1
			vec4(0.0, 1.0, 0.0, 0.0),

			vec4(0.0, 1.0, 0.0, 0.0),
			vec4(0.0, 1.0, 0.0, 0.0),	// 3
			vec4(0.0, 1.0, 0.0, 0.0),
			
			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 7
			vec4(0.0, 0.0, 1.0, 0.0),

			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),	// 8
			vec4(0.0, 0.0, 1.0, 0.0),
			
			vec4(0.0, -1.0, 0.0, 0.0),
			vec4(0.0, -1.0, 0.0, 0.0),	// 6
			vec4(0.0, -1.0, 0.0, 0.0),

			vec4(0.0, -1.0, 0.0, 0.0),
			vec4(0.0, -1.0, 0.0, 0.0),	// 11
			vec4(0.0, -1.0, 0.0, 0.0)
		};

		std::vector<vec4> v;
		v.assign(verts, verts + 36);
		return v;
	}

	std::vector<vec2> Geometry::CreateCubeTextureCoordinates()
	{
		std::vector<vec2> texCoords;

		for (int i = 0; i < 12; ++i)
		{
			if (i % 2)
			{
				texCoords.push_back(vec2(0.0, 0.0));
				texCoords.push_back(vec2(0.0, 1.0));
				texCoords.push_back(vec2(1.0, 1.0));
			}
			else
			{
				texCoords.push_back(vec2(1.0, 1.0));
				texCoords.push_back(vec2(1.0, 0.0));
				texCoords.push_back(vec2(0.0, 0.0));
			}
		}

		//std::swap(texCoords[1], texCoords[4]);

		return texCoords;
	}

	std::vector<vec2> Geometry::CreateSkyboxTextureCoordinates()
	{
		std::vector<vec2> texCoords;

		// FRONT
		/*texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));*/

		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 1.0 / 3.0));

		// BACK
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.75, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.75, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.75, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.75, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.75, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.75, 1.0 / 3.0));

		// LEFT
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.0, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.0, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.0, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.0, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.0, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.0, 1.0 / 3.0));

		// BOTTOM
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));

		/*texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 2.0 / 3.0));*/


		// RIGHT
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.5, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.5, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.5, 1.0 / 3.0));

		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.5, 1.0 / 3.0));
		texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.5, 1.0 / 3.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.5, 1.0 / 3.0));

		// TOP
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 0.0));
		texCoords.push_back(vec2(0.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 0.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 0.0));
		texCoords.push_back(vec2(1.0, 1.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 0.0));
		texCoords.push_back(vec2(1.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 0.0));
		texCoords.push_back(vec2(0.0, 0.0) * vec2(0.25, 1.0 / 3.0) + vec2(0.25, 0.0));

		return texCoords;
	}

	std::vector<vec4> Geometry::CreateRectangle(float size)
	{
		std::vector<vec4> v;
		v.push_back(vec4(-size / 2.0, -size / 2.0, 0.0, 1.0));
		v.push_back(vec4(size / 2.0, -size / 2.0, 0.0, 1.0));
		v.push_back(vec4(-size / 2.0, size / 2.0, 0.0, 1.0));
		v.push_back(vec4(size / 2.0, size / 2.0, 0.0, 1.0));
		return v;
	}

	std::vector<vec4> Geometry::CreateRectangleGeometry()
	{
		std::vector<vec4> v;
		v.push_back(vec4(-1.0 / 2.0, 1.0 / 2.0, 0.0, 1.0));
		v.push_back(vec4(-1.0 / 2.0, -1.0 / 2.0, 0.0, 1.0));
		v.push_back(vec4(1.0 / 2.0, -1.0 / 2.0, 0.0, 1.0));
		v.push_back(vec4(1.0 / 2.0, 1.0 / 2.0, 0.0, 1.0));
		return v;
	}

	std::vector<vec4> Geometry::CreatePrism(const std::vector<vec4>& originalShape, std::vector<vec4>& normals)
	{
		normals.clear();
		std::vector<vec4> shape;

		std::vector<vec4> ring1 = originalShape;
		std::vector<vec4> ring2 = Util::TransformVertices(originalShape, translate(vec3(0.0, 0.0, -1.0)));

		for (int i = 0; i < ring1.size(); i++)
		{
			shape.push_back(vec4(0.0, 0.0, 0.0, 1.0));
			shape.push_back(ring1[i]);
			normals.push_back(vec4(0.0, 0.0, 1.0, 0.0));
			normals.push_back(vec4(0.0, 0.0, 1.0, 0.0));
		}

		for (int i = 0; i < ring1.size(); i++)
		{
			shape.push_back(ring1[i]);
			shape.push_back(ring2[i]);
			normals.push_back(vec4(ring1[i].x, ring1[i].y, ring1[i].z, 0.0));
			normals.push_back(vec4(ring1[i].x, ring1[i].y, ring1[i].z, 0.0));
		}

		for (int i = 0; i < ring2.size(); i++)
		{
			shape.push_back(ring2[i]);
			shape.push_back(vec4(0.0, 0.0, -1.0, 1.0));
			normals.push_back(vec4(0.0, 0.0, -1.0, 0.0));
			normals.push_back(vec4(0.0, 0.0, -1.0, 0.0));
		}

		return shape;
	}

	std::vector<vec4> Geometry::CreateCylinderNormals(const std::vector<vec4>& circle)
	{
		std::vector<vec4> normals;

		for (int i = 0; i < circle.size() * 2; i++)
			normals.push_back(vec4(0.0, 0.0, 1.0, 0.0));
		for (int i = 0; i < circle.size(); i++)
		{
			normals.push_back(normalize(circle[i]));
			normals.push_back(normalize(circle[i]));
		}
		normals.push_back(vec4(1.0, 0.0, 0.0, 0.0));
		normals.push_back(vec4(1.0, 0.0, 0.0, 0.0));
		for (int i = 0; i < circle.size() * 2; i++)
			normals.push_back(vec4(0.0, 0.0, -1.0, 0.0));

		return normals;
	}

	std::vector<vec4> Geometry::CreateSphereNormals(std::vector<vec4> sphere)
	{
		std::vector<vec4> normals;
		for (std::vector<vec4>::iterator it = sphere.begin(); it != sphere.end(); ++it)
		{
			vec4 normal = *it;
			normal.w = 0.0;
			normals.push_back(normalize(normal));
		}
		return normals;
	}
}
