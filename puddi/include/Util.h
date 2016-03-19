#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <GL/glew.h>
#include "GlmHeaders.h"
#include <SDL2/SDL.h>

const GLdouble DegreesToRadians = M_PI / 180.0;

namespace puddi
{
	class Line;

	namespace Util
	{
		/**********************************************************************
		Function:		TransformVertices
		Purpose:		Return a copy of a std::vector of vertices with the
                        given transform applied to each
		Parameters:		const std::vector<vec4>& vertices - input vertices
                        const mat4& transform - transform to apply to vertices
		**********************************************************************/
		std::vector<vec4> TransformVertices(const std::vector<vec4>& vertices, const mat4& transform);

        float Length(const glm::vec2 &v);

		float Length(const glm::vec4 &v);

		glm::vec2 Normalize(const glm::vec2 &v);

		glm::vec4 Normalize(const glm::vec4 &v);

		/**********************************************************************
		Function:		Distance
		Purpose:		Compute the distance between two given points.
		Parameters:		vec4 p1 - point 1
                        vec4 p2 - point 2
		**********************************************************************/
		float Distance(const vec4& p1, const vec4& p2);

		/**********************************************************************
		Function:		Distance
		Purpose:		Compute the distance between two given points.
		Parameters:		vec2 p1 - point 1
                        vec2 p2 - point 2
		**********************************************************************/
		float Distance(const vec2& p1, const vec2& p2);

		/**********************************************************************
		Function:		DistanceSquared
		Purpose:		Compute the distance between two given points squared.
		Parameters:		vec4 p1 - point 1
                        vec4 p2 - point 2
		**********************************************************************/
		float DistanceSquared(const vec4& p1, const vec4& p2);

		/**********************************************************************
		Function:		DistanceSquared
		Purpose:		Compute the distance between two given points squared.
		Parameters:		vec2 p1 - point 1
                        vec2 p2 - point 2
		**********************************************************************/
		float DistanceSquared(const vec2& p1, const vec2& p2);

		/**********************************************************************
		Function:		TimeToString
		Purpose:		Converts a time value in milliseconds to a formatted
                        string.
		Parameters:		int time - time value in milliseconds
		**********************************************************************/
		std::string TimeToString(int time);

		/**********************************************************************
		Function:		RotateX
		Purpose:		Compute the rotation matrix to rotate about the X axis
                        by the given amount in radians
		Parameters:		float theta - x rotation amount
		**********************************************************************/
		mat4 RotateX(float theta);

		/**********************************************************************
		Function:		RotateY
		Purpose:		Compute the rotation matrix to rotate about the Y axis
                        by the given amount in radians
		Parameters:		float theta - y rotation amount
		**********************************************************************/
		mat4 RotateY(float theta);

		/**********************************************************************
		Function:		RotateZ
		Purpose:		Compute the rotation matrix to rotate about the Z axis
                        by the given amount in radians
		Parameters:		float theta - z rotation amount
		**********************************************************************/
		mat4 RotateZ(float theta);

		mat4 InverseRotateX(const mat4& rotateX);

		mat4 InverseRotateY(const mat4& rotateY);

		mat4 InverseRotateZ(const mat4& rotateZ);

		mat4 InverseScale(const mat4& scale);

		/**********************************************************************
		Function:		ToString
		Purpose:		Convert an object to its string representation
                        via its insertion operator.
        Parameters:     const T& value - value to convert to string
		**********************************************************************/
		template<class T>
		std::string ToString(const T& value)
		{
			std::stringstream sstream;
			sstream << value;
			return sstream.str();
		}

		/**********************************************************************
		Function:		ToString
		Purpose:		Convert an object to its string representation with up
                        to a given number of leading zeroes
        Parameters:     const T& value - value to convert to string
                        const int setw - guaranteed # of characters
		**********************************************************************/
		template<class T>
		std::string ToStringWithLeadingZeroes(const T& value, const int setw)
		{
			std::stringstream sstream;
			sstream << std::setfill('0') << std::setw(setw) << value;
			return sstream.str();
		}

		mat4 LookAt(const vec3& eye, const vec3& at, const vec3& up);

		mat4 Perspective(const GLfloat fovy, const GLfloat aspect, const GLfloat zNear, const GLfloat zFar);

		std::vector<vec2> Vec4toVec2(const std::vector<vec4>& v);

		void Apply(std::vector<vec4>& vecs, vec4(*func)(const vec4&));

		std::vector<char> ReadAllBytes(std::string fileName);

		float WrapAngle(float theta);

		bool FileExists(const std::string& name);

		glm::vec4 InterpolateRainbow(float val, float alpha);
	};
}

#endif
