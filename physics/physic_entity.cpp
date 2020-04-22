#include "physic_entity.hpp"
#include "../maths/math_intersection.hpp"

// --------------------------------------------------------------------------
Entity::Entity(Vec2 p, float m, float r, float f)
    : mass(m)
    , friction(f)
    , restitution(r)
    , v_angular(0.0)
    , position(p)
    , rotation(0.0)
{}

// --------------------------------------------------------------------------
Entity::~Entity() {}




// --------------------------------------------------------------------------
CircleEntity::CircleEntity(Vec2 p, float r, float m)
    : Entity(p,m)
    , Circle(p,r)
{}

// --------------------------------------------------------------------------
CircleEntity::~CircleEntity() {}




// --------------------------------------------------------------------------
RectEntity::RectEntity(Vec2 p, float w,float h,float m)
    : Entity(p,m)
    , Polygon(w,h)
    , width(w)
    , height(h)
    , dirty(true)
    , baseModel(w,h)
{}

// --------------------------------------------------------------------------
RectEntity::~RectEntity() {}

// --------------------------------------------------------------------------
void RectEntity::update()
{
    if(dirty)
    {
        clone(baseModel);
        rotate(rotation);
        move(position);
        dirty = false;
    }
}

// --------------------------------------------------------------------------
void RectEntity::change() { dirty = true; }



// --------------------------------------------------------------------------
GroupEntity::GroupEntity() : Entity() {}

// --------------------------------------------------------------------------
GroupEntity::~GroupEntity() {}

// --------------------------------------------------------------------------
void GroupEntity::compose(Entity* e)
{
    entities.push_back(e);
}



// --------------------------------------------------------------------------
BoxEntity::BoxEntity(float w, float h, float t, Vec2 p, float m)
    : GroupEntity()
{
    compose( new RectEntity(p-Vec2(w*0.5f,0.f), t, h, m*0.25f) );
    compose( new RectEntity(p+Vec2(w*0.5f,0.f), t, h, m*0.25f) );
    compose( new RectEntity(p-Vec2(0.f,h*0.5f), w, t, m*0.25f) );
    compose( new RectEntity(p+Vec2(0.f,h*0.5f), w, t, m*0.25f) );
}

// --------------------------------------------------------------------------
BoxEntity::~BoxEntity() {}




// --------------------------------------------------------------------------
Vec2 projectOnEdge(const RectEntity& r, const Vec2& p)
{
    const float EPSILON = 10.0;
    float maxEdgeDist = len( Vec2(r.width,r.height) );
    Vec2 ray = normalize(p) * maxEdgeDist * (1.f+EPSILON);
    
    Arr<Vec2> res_p, res_n;
    Seg2Poly(r.position, r.position+ray, r, res_p, res_n);
    
    if(res_p.empty()) return normalize(p) * maxEdgeDist;
    return res_p[0] - r.position;
}

// --------------------------------------------------------------------------
Vec2 averagePosition(const Arr<Vec2>& arr)
{
    Vec2 res;
    for(auto v : arr) res += v;
    return res / (float)arr.size();
}

// --------------------------------------------------------------------------
Vec2 averageNormal(const Arr<Vec2>& arr)
{
    Vec2 res;
    for(auto v : arr) res += v;
    if( len2(res) == 0.f && !arr.empty() ) res = arr[0];
    return normalize(res);
}



// --------------------------------------------------------------------------
bool Entity2Entity(const Entity& e1, const Entity& e2, CollisionData& out_coll)
{
    const RectEntity* r1 = dynamic_cast< const RectEntity* >( &e1 );
    const RectEntity* r2 = dynamic_cast< const RectEntity* >( &e2 );
    const CircleEntity* c1 = dynamic_cast< const CircleEntity* >( &e1 );
    const CircleEntity* c2 = dynamic_cast< const CircleEntity* >( &e2 );
    // GroupEntity* g1 = dynamic_cast< const GroupEntity* >(&e1);
    const GroupEntity* g2 = dynamic_cast< const GroupEntity* >(&e2);
    
    if(g2)
    {
        for(auto& e2bis : g2->entities) 
        {
            if(e1.mass == 0.f && e2bis->mass == 0.f) continue;
            if( Entity2Entity(e1,*e2bis,out_coll) ) return true;
        }
        return false;
    }
    
    if(e1.mass == 0.f && e2.mass == 0.f) return false;
    if( r1 && r2 && Rect2Rect(*r1, *r2, out_coll) ) return true;
    if( c1 && c2 && Circle2Circle(*c1, *c2, out_coll) ) return true;
    if( c1 && r2 && Circle2Rect(*c1, *r2, out_coll) ) return true;
    
    return false;
}

// --------------------------------------------------------------------------
bool Circle2Circle(const CircleEntity& c1, const CircleEntity& c2, CollisionData& res)
{
    Vec2 dir = c2.position - c1.position;
    float d = len(dir);
    float th = c1.radius+c2.radius;
    float penetration = th - d;
    if( penetration > 0.f )
    {
        res.e1 = const_cast<CircleEntity*>( &c1 );
        res.e2 = const_cast<CircleEntity*>( &c2 );
        res.penetration = penetration;
        res.normal1 = normalize( dir );
        res.normal2 = -res.normal1;
        res.hitPoint = (c1.position + c2.position) * 0.5f;
        return true;
    }
    
    return false;
}

// --------------------------------------------------------------------------
bool Rect2Rect(const RectEntity& r1, const RectEntity& r2, CollisionData& res)
{
    Arr<Vec2> res_p;
    Arr<Vec2> res_n1;
    Arr<Vec2> res_n2;
    
    if( Poly2Poly(r1, r2, res_p, res_n1, res_n2) )
    {
        if(res_p.empty()) return false;

        Vec2 hitPoint = averagePosition(res_p);
        Vec2 n1,n2;
        if(!res_n1.empty()) n1 = averageNormal(res_n1);
        else n1 = normalize(hitPoint - r1.position);
        if(!res_n2.empty()) n2 = averageNormal(res_n2);
        else n2 = normalize(hitPoint - r2.position);
        // Vec2 n1 = averageNormal(res_n1);
        // Vec2 n2 = averageNormal(res_n2);
        
        res.e1 = const_cast<RectEntity*>( &r1 );
        res.e2 = const_cast<RectEntity*>( &r2 );
        Vec2 r1k = hitPoint - r1.position;
        Vec2 r2k = hitPoint - r2.position;
        
        Vec2 r1Edge = projectOnEdge(r1,r1k);
        Vec2 r2Edge = projectOnEdge(r2,r2k);
        res.penetration = dot(n2,r2Edge) + dot(n1,r1Edge) - (dot(n2,r2k)+dot(n1,r1k));
        if(res.penetration < 0.f) return false;
        
        res.normal1 = n1;
        res.normal2 = n2;
        
        res.hitPoint = hitPoint;
        return true;
    }
    
    return false;
}

// --------------------------------------------------------------------------
bool Circle2Rect(const CircleEntity& c, const RectEntity& r, CollisionData& res)
{
    Arr<Vec2> colli_p, colli_n;
    if( Circle2Poly(c, r, colli_p, colli_n) )
    {
        if(colli_p.empty()) return false;

        Vec2 hitPoint = averagePosition(colli_p);
        Vec2 r_normal;
        if(!colli_n.empty()) r_normal = averageNormal(colli_n);
        else r_normal = normalize(r.position - c.position);
        
        Vec2 ck = hitPoint - c.position;
        Vec2 rk = hitPoint - r.position;
        
        Vec2 c_normal = normalize(ck);
        Vec2 rectEdge = projectOnEdge(r, rk);
        
        res.e1 = const_cast<CircleEntity*>( &c );
        res.e2 = const_cast<RectEntity*>( &r );
        res.penetration = dot(r_normal,rectEdge) + c.radius - (len(ck)+dot(r_normal,rk));
        
        res.normal1 = c_normal;
        res.normal2 = r_normal;
        
        res.hitPoint = hitPoint;
        return true;
    }
    return false;
}
