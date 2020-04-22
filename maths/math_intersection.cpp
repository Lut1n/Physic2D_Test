#include "math_intersection.hpp"
#include <cmath>
#include <iostream>

// --------------------------------------------------------------------------
bool Circle2Circle(const Circle& c1, const Circle& c2, Arr<Vec2>& out_p)
{
    Vec2 dir = c2.center - c1.center;
    float d = len(dir);
    if( c1.radius+c2.radius - d > 0.f )
    {
        Vec2 ccCenter = c1.center + dir*0.5f;
        
        float dirOffset = d * 0.5f;
        float delta = c1.radius*c1.radius - dirOffset*dirOffset;
        float tanOffset = std::sqrt( delta );
        Vec2 tanDir = getNormal(dir) * tanOffset;
        
        out_p.push_back( ccCenter - tanDir );
        out_p.push_back( ccCenter + tanDir );
        
        return true;
    }
    
    return false;
}

// --------------------------------------------------------------------------
bool getIntersectionPoints(const Polygon& p1, const Polygon& p2, Arr<Vec2>& out_p, Arr<Vec2>& out_n1, Arr<Vec2>& out_n2)
{
    if( p1.vertices.empty() || p2.vertices.empty() ) return false;
    
    bool hit = false;
    Vec2 prev = p1.vertices[p1.vertices.size()-1];
    for(auto ve : p1.vertices)
    {
        if( Seg2Poly(prev, ve, p2, out_p, out_n2) )
        {
            out_n1.push_back( getNormal(prev,ve) );
            hit = true;
        }
        
        prev = ve;
    }
    return hit;
}

// --------------------------------------------------------------------------
bool getInsidePoints(const Polygon& p1, const Polygon& p2, Arr<Vec2>& out_p, Arr<Vec2>& out_n)
{
    if( p1.vertices.empty() || p2.vertices.empty() ) return false;
    
    bool hit = false;
    Vec2 prev = p1.vertices[p1.vertices.size()-1];
    Vec2 prev2 = p1.vertices[p1.vertices.size()-2];
    for(auto ve : p1.vertices)
    {
        if( inside(prev, p2) )
        {
            Vec2 resN = getNormal(prev2,prev) + getNormal(prev,ve); resN *= 0.5f;
            out_p.push_back(prev);
            out_n.push_back( resN );
            hit = true;
        }
        
        prev2 = prev;
        prev = ve;
    }
    
    return hit;
}

// --------------------------------------------------------------------------
bool Poly2Poly(const Polygon& p1, const Polygon& p2, Arr<Vec2>& out_p, Arr<Vec2>& out_n1, Arr<Vec2>& out_n2)
{
    bool hit = false;
    
    hit |= getIntersectionPoints(p1,p2,out_p,out_n1,out_n2);
    hit |= getInsidePoints(p1,p2,out_p,out_n1);
    hit |= getInsidePoints(p2,p1,out_p,out_n2);
    
    return hit;
}

// --------------------------------------------------------------------------
bool Circle2Line(const Circle& c, const Vec2& l1, const Vec2& l2, Arr<Vec2>& out_p)
{
    Vec2 cl1 = l1;//-c.center;
    Vec2 cl2 = l2;//-c.center;
    
    // Vec2 cl1 = l1-c.center;
    // Vec2 cl2 = l2-c.center;
    
    Vec2 d = cl2 - cl1;
    float dr2 = dot(d,d);
    float D = cl1.x*cl2.y - cl2.x*cl1.y;
    
    float delta = c.radius*c.radius * dr2 - D*D;
    
    const float EPSILON = 0.01;
    
    // if(delta < -EPSILON) // no intersection
    if(delta < 0.f)
    {
        return false;
    }
    else if(delta > EPSILON) // two intersections
    {
        float sqrtDelta = std::sqrt( delta );
        
        Vec2 i1, i2;
        i1.x = ( D*d.y - sign(d.y) * d.x * sqrtDelta ) / dr2;
        i2.x = ( D*d.y + sign(d.y) * d.x * sqrtDelta ) / dr2;
        
        i1.y = ( -D*d.x - std::abs(d.y) * sqrtDelta ) / dr2;
        i2.y = ( -D*d.x + std::abs(d.y) * sqrtDelta ) / dr2;
        
        out_p.push_back(i1);
        out_p.push_back(i2);
        
        // out_p.push_back(c.center + i1);
        // out_p.push_back(c.center + i2);
        
        return true;
    }
    /*else // one intersection
    {
        Vec2 i1;
        i1.x = ( D*d.y - sign(d.y) * d.x * sqrtDelta ) / dr2;
        i1.y = ( -D*d.x - std::abs(d.y) * sqrtDelta ) / dr2;
        
        out_p.push_back(c.center + i1);
        
        return true;
    }*/
}


// --------------------------------------------------------------------------
bool Circle2Seg(const Circle& c, const Vec2& s1, const Vec2& s2, Arr<Vec2>& out_p)
{
    Arr<Vec2> local_res;
    
    Vec2 l1 = s1-c.center;
    Vec2 l2 = s2-c.center;
    
    if( Circle2Line(c,l1,l2,local_res) )
    {
        if(local_res.empty()) return false;

        Vec2 dif = l2-l1;
        Vec2 d = normalize(dif);
        float dist = len(dif);
        
        float t1 = dot(d,local_res[0]-l1) / dist;
        float t2 = dot(d,local_res[1]-l1) / dist;
        
        bool hit = false;
        if(t1>0.f && t1<1.f) { out_p.push_back(c.center+local_res[0]); hit=true; }
        if(t2>0.f && t2<1.f) { out_p.push_back(c.center+local_res[1]); hit=true; }
        
        return hit;
    }
    return false;
}

// --------------------------------------------------------------------------
bool Circle2Poly(const Circle& c, const Polygon& p, Arr<Vec2>& out_p, Arr<Vec2>& out_n)
{
    if( p.vertices.empty() ) return false;
    
    bool hit = false;
    Arr<Vec2> local_res;
    Vec2 prev = p.vertices[p.vertices.size()-1];
    for(auto ve : p.vertices)
    {
        if( Circle2Seg(c,prev,ve,local_res) )
        {
            for(auto local_it : local_res) out_p.push_back(local_it);
            out_n.push_back( getNormal(prev,ve) );
            hit = true;
        }
        
        prev = ve;
    }
    
    return hit;
}

// --------------------------------------------------------------------------
bool Seg2Seg(const Vec2& s1a, const Vec2& s1b, const Vec2& s2a, const Vec2& s2b, Vec2& out_p)
{
    Vec2 rs1b = s1b-s1a;
    Vec2 rs2a = s2a-s1a;
    Vec2 rs2b = s2b-s1a;
    
    Vec2 tang = normalize(rs1b);
    Vec2 n = getNormal(rs1b);
    
    double da = dot(rs2a,n);
    double db = dot(rs2b,n);
    if( sign(da) != sign(db) ) // one on each side
    {
        double t = da / (da - db);
        out_p = mix(rs2a, rs2b, t);
        
        double di = dot(out_p,tang);
        if( di > 0.0 && di < len(rs1b) )
        {
            out_p = s1a + out_p;
            return true;
        }
    }
    // test degenerated case (parallel segment very closed)
    // else if()
    // {
    //
    // }
    
    return false;
}

// --------------------------------------------------------------------------
bool Seg2Poly(const Vec2& sa, const Vec2& sb, const Polygon& p, Arr<Vec2>& out_p, Arr<Vec2>& out_n)
{
    if( p.vertices.empty() ) return false;
    
    bool hit = false;
    Vec2 local_res;
    Vec2 prev = p.vertices[p.vertices.size()-1];
    for(auto ve : p.vertices)
    {
        if( Seg2Seg(sa,sb,prev,ve,local_res) )
        {
            out_p.push_back(local_res);
            out_n.push_back( getNormal(prev,ve) );
            hit = true;
        }
        
        prev = ve;
    }
    
    return hit;
}

// --------------------------------------------------------------------------
bool inside(const Vec2& v, const Polygon& p)
{
    if( p.vertices.empty() ) return false;
    
    bool res = true;
    Vec2 prev = p.vertices[p.vertices.size()-1];
    for(auto ve : p.vertices)
    {
        res &= above(v,prev,ve);
        prev = ve;
    }
    
    return res;
}
