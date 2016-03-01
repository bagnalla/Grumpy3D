/*************************************************************************************

Program:			CS 4250 Final Project

Author:				Alexander Bagnall
Email:				ab667712@ohio.edu

Description:		Implementation file for the Util class.

Date:				December 9, 2015

*************************************************************************************/

#include "Util.h"
#include <glm/gtx/color_space.hpp>
#include <fstream>
#include <iostream>

namespace puddi
{
	// PUBLIC

	std::vector<vec4> Util::TransformVertices(const std::vector<vec4>& vertices, const mat4& transform)
	{
		std::vector<vec4> transformedVertices;
		for (std::vector<vec4>::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			transformedVertices.push_back(transform * *it);
		}
		return transformedVertices;
	}

    float Util::Length(const vec2& v)
    {
        if (v.x != 0.0f || v.y != 0.0f)
            return length(v);
        return 0.0f;
    }

    float Util::Length(const vec4& v)
    {
//        if (v.x != 0.0f || v.y != 0.0f || v.z != 0.0f || v.w != 0.0f)
//            return length(v);
//        return 0.0f;
        float l = length(v);
        if (l == l)
            return l;
        else
            return 0.0f;
    }

    glm::vec2 Util::Normalize(const glm::vec2 &v)
    {
        if (v.x != 0.0f || v.y != 0.0f)
            return normalize(v);
        else
            return vec2(0.0f, 0.0f);
    }

    glm::vec4 Util::Normalize(const glm::vec4 &v)
    {
        if (v.x != 0.0f || v.y != 0.0f || v.z != 0.0f || v.w != 0.0f)
            return normalize(v);
        else
            return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

	float Util::Distance(const vec4& p1, const vec4& p2)
	{
		//return length(p1 - p2);
		vec4 d = p2 - p1;
		return std::sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
	}

	float Util::Distance(const vec2& p1, const vec2& p2)
	{
		return length(p1 - p2);
	}

	float Util::DistanceSquared(const vec4& p1, const vec4& p2)
	{
		/*vec4 displacement = p2 - p1;
		return dot(displacement, displacement);*/
		vec4 d = p2 - p1;
		return d.x*d.x + d.y*d.y + d.z*d.z;
	}

	float Util::DistanceSquared(const vec2& p1, const vec2& p2)
	{
		vec2 displacement = p2 - p1;
		return dot(displacement, displacement);
	}

	std::string Util::TimeToString(int time)
	{
		time /= 1000;

		int seconds = time % 60;
		time /= 60;

		int minutes = time % 60;
		time /= 60;

		return (time != 0 ? ToStringWithLeadingZeroes(time, 2) + ":" : "")
			+ ToStringWithLeadingZeroes(minutes, 2) + ":"
			+ ToStringWithLeadingZeroes(seconds, 2);
	}

	mat4 Util::RotateX(float theta)
	{
		mat4 c;
		c[2][2] = c[1][1] = cos(theta);
		c[2][1] = sin(theta);
		c[1][2] = -c[2][1];
		return c;
	}

	mat4 Util::RotateY(float theta)
	{
		mat4 c;
		c[2][2] = c[0][0] = cos(theta);
		c[0][2] = sin(theta);
		c[2][0] = -c[0][2];
		return c;
	}

	mat4 Util::RotateZ(float theta)
	{
		mat4 c;
		c[0][0] = c[1][1] = cos(theta);
		c[1][0] = sin(theta);
		c[0][1] = -c[1][0];
		return c;
	}

	mat4 Util::InverseRotateX(const mat4& rotateX)
	{
		mat4 r = rotateX;
		r[2][1] = -r[2][1];
		r[1][2] = -r[1][2];
		return r;
	}

	mat4 Util::InverseRotateY(const mat4& rotateY)
	{
		mat4 r = rotateY;
		r[0][2] = -r[0][2];
		r[2][0] = -r[2][0];
		return r;
	}

	mat4 Util::InverseRotateZ(const mat4& rotatZ)
	{
		mat4 r = rotatZ;
		r[1][0] = -r[1][0];
		r[0][1] = -r[0][1];
		return r;
	}

	mat4 Util::InverseScale(const mat4& scale)
	{
		mat4 s;
		s[0][0] = 1.0f / s[0][0];
		s[1][1] = 1.0f / s[1][1];
		s[2][2] = 1.0f / s[2][2];
		return s;
	}

	mat4 Util::LookAt(const vec3& eye, const vec3& at, const vec3& up)
	{
		vec3 n = normalize(eye - at);
		vec3 u = normalize(cross(up, n));
		vec3 v = normalize(cross(n, u));
		vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
		mat4 c = mat4(vec4(u, 0.0f), vec4(v, 0.0f), vec4(n, 0.0f), t);
		return c * translate(-eye);
	}

	mat4 Util::Perspective(const GLfloat fovy, const GLfloat aspect, const GLfloat zNear, const GLfloat zFar)
	{
		GLfloat top = tan(fovy*DegreesToRadians / 2) * zNear;
		GLfloat right = top * aspect;

		mat4 c;
		c[0][0] = zNear / right;
		c[1][1] = zNear / -top;
		c[2][2] = -(zFar + zNear) / (zFar - zNear);
		c[2][3] = -2.0*zFar*zNear / (zFar - zNear);
		c[3][2] = -1.0;
		c[3][3] = 0.0;
		return c;
	}

	std::vector<vec2> Util::Vec4toVec2(const std::vector<vec4>& v)
	{
		std::vector<vec2> v2;
		for (auto it = v.begin(); it != v.end(); ++it)
			v2.push_back(vec2((*it).x, (*it).y));
		return v2;
	}

	void Util::Apply(std::vector<vec4>& vecs, vec4(*func)(const vec4&))
	{
		for (int i = 0; i < vecs.size(); ++i)
			vecs[i] = func(vecs[i]);
	}

	std::vector<char> Util::ReadAllBytes(char const* fileName)
	{
		if (!FileExists(fileName))
		{
			std::cerr << "File not found: " << fileName << std::endl;
			return std::vector<char>();
		}

		std::ifstream ifs(fileName, std::ios::binary | std::ios::ate);
		std::ifstream::pos_type pos = ifs.tellg();

		std::vector<char>  result(pos);

		ifs.seekg(0, std::ios::beg);
		ifs.read(&result[0], pos);

		return result;
	}

	float Util::WrapAngle(float theta)
	{
		//theta = fmod(theta, M_PI_2);
		//if (theta < 0)
			//theta += M_PI_2;
		return theta;
	}

	bool Util::FileExists(const std::string& name) {
		std::ifstream f(name.c_str());
		if (f.good()) {
			f.close();
			return true;
		}
		else {
			f.close();
			return false;
		}
	}

	glm::vec4 Util::InterpolateRainbow(float val, float alpha)
	{
//        auto hsv = glm::hsvColor(vec3(0.5f, 0.5f, 0.5f));
//        hsv.x = val;
//        auto temp = glm::rgbColor(hsv);
//        std::cout << temp.x << std::endl;
//        std::cout << temp.y << std::endl;
//        std::cout << temp.z << std::endl;
//        return vec4(temp.x, temp.y, temp.z, alpha);

        return vec4(1 - val, 1 - val, 1 - val, alpha);
	}
}
