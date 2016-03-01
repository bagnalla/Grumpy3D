#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include "GlmHeaders.h"

namespace puddi
{
	class Geometry
	{
	public:
		/**********************************************************************
		Function:		CreateCatmullRomCurve
		Purpose:		Generate vertices along a curve using a Catmull-Rom
		spline function.
		Parameters:		vec4 previousPoint - control point 0
		vec4 point1 - control point 1
		vec4 point2 - control point 2
		vec4 afterPoint - control point 3
		float curviness - determines the number of points along
		the curve to generate
		Calls:			CRSpline::Eq
		**********************************************************************/
		static std::vector<vec4> CreateCatmullRomCurve(const vec4& p0, const vec4& p1, const vec4& p2, const vec4& p3, float curviness);

		/**********************************************************************
		Function:		CreateBlob
		Purpose:		Generate a "blob" of vertices based on a set of input
		vertices. The resulting set of vertices will be similar
		to the input but with extra points along curves around
		them.
		Parameters:		float curviness - determines the number of points along
		the curves to generate
		vec4 verts[] - the input vertices
		int numOfVerts - the number of input vertices
		Preconditions:	There must be at least three input vertices.
		Calls:			CRSpline::Eq
		**********************************************************************/
		static std::vector<vec4> CreateBlob(float curviness, const vec4 verts[], int numOfVerts);

		/**********************************************************************
		Function:		CreateCircle
		Purpose:		Generate a circle of vertices around a given point
		with a given radius.
		Parameters:		vec4 position - center position of the circle
		float radius - radius of the circle
		int numberOfVertices - the # of vertices to generate
		**********************************************************************/
		static std::vector<vec4> CreateCircle(int numberOfVertices);

		static std::vector<vec4> CreateCircle(int numberOfVertices, std::vector<vec4>& tangents);

		/**********************************************************************
		Function:		CreateSphere
		Purpose:		Generate the vertices for a sphere using a rotation
		method.
		Parameters:		int definition
		Calls:			CreateCircle
		**********************************************************************/
		static std::vector<vec4> CreateSphere(int definition);

		static std::vector<vec4> CreateSphere(int definition, std::vector<vec4>& tangents, std::vector<vec4>& binormals);

		/**********************************************************************
		Function:		CreateSphere2
		Purpose:		Generate the vertices for a sphere using recursive
		subdivision of a tetrahedron. Based on the code from
		the Angel book.
		Parameters:		int definition
		**********************************************************************/
		static std::vector<vec4> CreateSphere2(int definition);

		/**********************************************************************
		Function:		CreateCircle
		Purpose:		Generate the vertices for a cube.
		Parameters:		float size - the height/width/depth of the cube
		**********************************************************************/
		static std::vector<vec4> CreateCube(float size);

		/**********************************************************************
		Function:		CreateCubeNormals
		Purpose:		Generate the normals for a cube.
		**********************************************************************/
		static std::vector<vec4> CreateCubeNormals();

		static std::vector<vec4> CreateCubeTangents();

		static std::vector<vec4> CreateCubeBiNormals();

		static std::vector<vec2> CreateCubeTextureCoordinates();

		static std::vector<vec2> CreateSkyboxTextureCoordinates();

		/**********************************************************************
		Function:		CreateRectangle
		Purpose:		Generate the vertices for a rectangle to be drawn as
		a triangle strip
		Parameters:		float size - the height/width of the rectangle
		**********************************************************************/
		static std::vector<vec4> CreateRectangle(float size);

		/**********************************************************************
		Function:		CreateRectangleGeometry
		Purpose:		Generate the vertices for a rectangle to be used for
		CreateBlob.
		**********************************************************************/
		static std::vector<vec4> CreateRectangleGeometry();

		/**********************************************************************
		Function:		CreatePrism
		Purpose:		Generate the vertices and normals for a prism based on
		a 2D shape assumed to be on the x-y plane
		Parameters:		const std::vector<vec4>& originalShape - 2D shape
		std::vector<vec4>& normals - vector to put normals in
		Postcondition:	normals is cleared and then filled with the normals
		corresponding to the vertices that are returned
		**********************************************************************/
		static std::vector<vec4> CreatePrism(const std::vector<vec4>& originalShape, std::vector<vec4>& normals);

		/**********************************************************************
		Function:		CreateCylinderNormals
		Purpose:		Generate the normals for a cylinder based on the circle
		that the cylinder is generated from (by passing the
		circle to CreatePrism)
		Parameters:		const std::vector<vec4>& circle - the circle
		**********************************************************************/
		static std::vector<vec4> CreateCylinderNormals(const std::vector<vec4>& circle);

		/**********************************************************************
		Function:		CreateSphereNormals
		Purpose:		Generate the normals for a sphere. Basically just cuts
		the w component off of the points of the sphere.
		Parameters:		const std::vector<vec4>& circle - the circle
		**********************************************************************/
		static std::vector<vec4> CreateSphereNormals(std::vector<vec4> sphere);
	};
}

#endif
