/**
 * @file CRSpline.h
 * @author Alexander Bagnall
 * @brief 2D Catmull-rom spline implementation
 *
 * Taken from http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio
 */

#ifndef CRSPLINE_H
#define CRSPLINE_H

#include <vector>
#include "GlmHeaders.h"

namespace puddi
{
    class CRSpline
    {
    public:
        // Constructors and destructor
        CRSpline();
        CRSpline(const CRSpline&);
        ~CRSpline();

        // Operations
        void AddSplinePoint(const vec4& v);
        vec4 GetInterpolatedSplinePoint(float t);   // t = 0...1; 0=vp[0] ... 1=vp[max]
        int GetNumPoints();
        vec4& GetNthPoint(int n);

        // Static method for computing the Catmull-Rom parametric equation
        // given a time (t) and a vector quadruple (p1,p2,p3,p4).
        static vec4 Eq(float t, const vec4& p1, const vec4& p2, const vec4& p3, const vec4& p4);

    private:
        std::vector<vec4> vp;
        float delta_t;
    };
}

#endif
