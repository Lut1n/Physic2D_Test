#ifndef MATH_GEOMETRY_HPP
#define MATH_GEOMETRY_HPP

#include "math_vector.hpp"

// --------------------------------------------------------------------------
struct Shape
{
    Shape();
    virtual ~Shape();
};

// --------------------------------------------------------------------------
struct Circle : public Shape
{
    Vec2 center;
    float radius;
    
    Circle();
    Circle(const Vec2& c, float r);
    virtual ~Circle();
    
    void move(const Vec2& va);
};

// --------------------------------------------------------------------------
struct Polygon : public Shape
{
    Arr<Vec2> vertices;
    
    Polygon();
    Polygon(const Arr<Vec2>& v);
    Polygon(float w, float h, Vec2 p=Vec2() );
    virtual ~Polygon();
    
    void buildRect(float w, float h, Vec2 c=Vec2() );
    void clone(const Polygon& p);
    
    void insert(const Vec2& v);
    void move(const Vec2& v);
    void rotate(float r);
};

#endif // MATH_GEOMETRY_HPP
