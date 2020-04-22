#ifndef MATH_INTERSECTION_HPP
#define MATH_INTERSECTION_HPP

#include "math_geometry.hpp"


// --------------------------------------------------------------------------
// compute intersection points and normals between 2 circles
bool Circle2Circle(const Circle& c1, const Circle& c2, Arr<Vec2>& out_p);

// --------------------------------------------------------------------------
// compute intersection points and normals between edges of 2 polygons (does not detect inner edges)
// used by Poly2Poly
bool getIntersectionPoints(const Polygon& p1, const Polygon& p2, Arr<Vec2>& o_p, Arr<Vec2>& o_n1, Arr<Vec2>& o_n2);

// compute inside points and associated normals between 2 polygons
// used by Poly2Poly
bool getInsidePoints(const  Polygon& p1, const Polygon& p2, Arr<Vec2>& o_p, Arr<Vec2>& o_n);

// --------------------------------------------------------------------------
// compute intersection points and normals between 2 polygons
bool Poly2Poly(const Polygon& p1, const Polygon& p2, Arr<Vec2>& out_p, Arr<Vec2>& out_n1, Arr<Vec2>& out_n2);

// --------------------------------------------------------------------------
// compute intersection points between a circle and a line
bool Circle2Line(const Circle& c, const Vec2& l1, const Vec2& l2, Arr<Vec2>& out_p);

// --------------------------------------------------------------------------
// compute intersection points between a circle and a Segment
bool Circle2Seg(const Circle& c, const Vec2& s1, const Vec2& s2, Arr<Vec2>& out_p);

// --------------------------------------------------------------------------
// compute intersection points and normals between a circle and a polygon
bool Circle2Poly(const Circle& c, const Polygon& p, Arr<Vec2>& out_p, Arr<Vec2>& out_n);

// --------------------------------------------------------------------------
// compute intersection points between 2 segments
bool Seg2Seg(const Vec2& s1a, const Vec2& s1b, const Vec2& s2a, const Vec2& s2b, Vec2& out_p);

// --------------------------------------------------------------------------
// compute intersection points between a segment and a polygon
bool Seg2Poly(const Vec2& sa, const Vec2& sb, const Polygon& p, Arr<Vec2>& out_p, Arr<Vec2>& out_n);

// --------------------------------------------------------------------------
// test if a point v is inside a polygon
bool inside(const Vec2& v, const Polygon& p);

// --------------------------------------------------------------------------
// compute the projection of a direction on polygon's edges
// Vec2 projectOnEdge(const Polygon& p, const Vec2& dir, Vec2 origin = Vec2());


#endif // MATH_INTERSECTION_HPP
