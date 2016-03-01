#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <GL/glew.h>
#include "GlmHeaders.h"
#include <SDL2/SDL.h>

const GLfloat DegreesToRadians = M_PI / 180.0;

namespace puddi
{
	class Line;

	class Util
	{
	public:

		/**********************************************************************
		Function:		TransformVertices
		Purpose:		Return a copy of a std::vector of vertices with the
		given transform applied to each
		Parameters:		const std::vector<vec4>& vertices - input vertices
		const mat4& transform - transform to apply to vertices
		**********************************************************************/
		static std::vector<vec4> TransformVertices(const std::vector<vec4>& vertices, const mat4& transform);

        static float Length(const glm::vec2 &v);

		static float Length(const glm::vec4 &v);

		static glm::vec2 Normalize(const glm::vec2 &v);

		static glm::vec4 Normalize(const glm::vec4 &v);

		/**********************************************************************
		Function:		Distance
		Purpose:		Compute the distance between two given points.
		Parameters:		vec4 p1 - point 1
		vec4 p2 - point 2
		**********************************************************************/
		static float Distance(const vec4& p1, const vec4& p2);

		/**********************************************************************
		Function:		Distance
		Purpose:		Compute the distance between two given points.
		Parameters:		vec2 p1 - point 1
		vec2 p2 - point 2
		**********************************************************************/
		static float Distance(const vec2& p1, const vec2& p2);

		/**********************************************************************
		Function:		DistanceSquared
		Purpose:		Compute the distance between two given points squared.
		Parameters:		vec4 p1 - point 1
		vec4 p2 - point 2
		**********************************************************************/
		static float DistanceSquared(const vec4& p1, const vec4& p2);

		/**********************************************************************
		Function:		DistanceSquared
		Purpose:		Compute the distance between two given points squared.
		Parameters:		vec2 p1 - point 1
		vec2 p2 - point 2
		**********************************************************************/
		static float DistanceSquared(const vec2& p1, const vec2& p2);

		/**********************************************************************
		Function:		TimeToString
		Purpose:		Converts a time value in milliseconds to a formatted
		string.
		Parameters:		int time - time value in milliseconds
		**********************************************************************/
		static std::string TimeToString(int time);

		/**********************************************************************
		Function:		RotateX
		Purpose:		Compute the rotation matrix to rotate about the X axis
		by the given amount in radians
		Parameters:		float theta - x rotation amount
		**********************************************************************/
		static mat4 RotateX(float theta);

		/**********************************************************************
		Function:		RotateY
		Purpose:		Compute the rotation matrix to rotate about the Y axis
		by the given amount in radians
		Parameters:		float theta - y rotation amount
		**********************************************************************/
		static mat4 RotateY(float theta);

		/**********************************************************************
		Function:		RotateZ
		Purpose:		Compute the rotation matrix to rotate about the Z axis
		by the given amount in radians
		Parameters:		float theta - z rotation amount
		**********************************************************************/
		static mat4 RotateZ(float theta);

		static mat4 InverseRotateX(const mat4& rotateX);

		static mat4 InverseRotateY(const mat4& rotateY);

		static mat4 InverseRotateZ(const mat4& rotateZ);

		static mat4 InverseScale(const mat4& scale);

		/**********************************************************************
		Function:		ToString
		Purpose:		Convert an object to its string representation
		via its insertion operator.
		const T& value - value to convert to string
		**********************************************************************/
		template<class T>
		static std::string ToString(const T& value)
		{
			std::stringstream sstream;
			sstream << value;
			return sstream.str();
		}

		/**********************************************************************
		Function:		ToString
		Purpose:		Convert an object to its string representation with up
		to a given number of leading zeroes
		const T& value - value to convert to string
		const int setw - guaranteed # of characters
		**********************************************************************/
		template<class T>
		static std::string ToStringWithLeadingZeroes(const T& value, const int setw)
		{
			std::stringstream sstream;
			sstream << std::setfill('0') << std::setw(setw) << value;
			return sstream.str();
		}

		static mat4 LookAt(const vec3& eye, const vec3& at, const vec3& up);

		static mat4 Perspective(const GLfloat fovy, const GLfloat aspect, const GLfloat zNear, const GLfloat zFar);

		static std::vector<vec2> Vec4toVec2(const std::vector<vec4>& v);

		static void Apply(std::vector<vec4>& vecs, vec4(*func)(const vec4&));

		static std::vector<char> ReadAllBytes(char const* fileName);

		static float WrapAngle(float theta);

		static bool FileExists(const std::string& name);

		static glm::vec4 InterpolateRainbow(float val, float alpha);
	};
}

#endif
