#ifndef PHYSIC_ENTITY_HPP
#define PHYSIC_ENTITY_HPP

#include "../maths/math_geometry.hpp"


// --------------------------------------------------------------------------
// physical entity
struct Entity
{
    // weight of the entity
    float mass;
    
    // resistance on surface [0;+1]
    float friction;
    
    // bouncing effect (generally between 0.2 and 0.8)
    float restitution;
    
    // velocities
    float v_angular;
    Vec2 v_linear;
    
    // current position and orientation in the world
    Vec2 position;
    float rotation;
    
    // construtor
    // p : position
    // m : mass
    // r : restitution
    // f = friction
    Entity(Vec2 p = Vec2(0.f,0.f), float m = 1.f, float r = 0.5, float f = 0.4);
    virtual ~Entity();
};

// --------------------------------------------------------------------------
struct CollisionData
{
    // colliding entities
    Entity* e1;
    Entity* e2;
    
    // overlapping distance to canceled
    float penetration;
    
    // hit point and surface normals
    Vec2 hitPoint;
    Vec2 normal1;
    Vec2 normal2;
};

// --------------------------------------------------------------------------
struct CircleEntity : public Entity, public Circle
{
    // constructor
    // p : position
    // r : radius
    // m = mass
    CircleEntity(Vec2 p=Vec2(0.f,0.f), float r = 10.f, float m = 1.f);
    virtual ~CircleEntity();
};

// --------------------------------------------------------------------------
struct RectEntity : public Entity, public Polygon
{
    // rectangle model : width and height
    float width;
    float height;
    
    // flag for updating dynamic polygon model
    bool dirty;
    
    // base model polygon
    Polygon baseModel;
    
    // constructor
    // p : position
    // w : width
    // h : height
    // m : mass
    RectEntity(Vec2 p=Vec2(0.f,0.f), float w=20.f,float h=20.f,float m = 1.f);
    virtual ~RectEntity();
    
    // if dirty, update dynamic polygon model using base model, position and rotation
    void update();
    
    // make dirty
    void change();
};

// --------------------------------------------------------------------------
struct GroupEntity : public Entity
{
    // list of entities
    Arr<Entity*> entities;
    
    GroupEntity();
    virtual ~GroupEntity();
    
    // add an entity to the list
    void compose(Entity* e);
};

// --------------------------------------------------------------------------
// specialized GroupEntity
// build a box with 4 rectanles
struct BoxEntity : public GroupEntity
{
    // constructor
    // w : width
    // h : height
    // t : thickness
    // p : position
    // m : mass
    BoxEntity(float w=50.f, float h=50.f, float t=10.f, Vec2 p=Vec2(0.f,0.f), float m = 0.f);
    virtual ~BoxEntity();
};

// --------------------------------------------------------------------------
// generic collision test between 2 entities
bool Entity2Entity(const Entity& e1, const Entity& e2, CollisionData& res);

// --------------------------------------------------------------------------
// test collision between 2 circles
bool Circle2Circle(const CircleEntity& c1, const CircleEntity& c2, CollisionData& res);

// --------------------------------------------------------------------------
// test collision between 2 rectangles
bool Rect2Rect(const RectEntity& r1, const RectEntity& r2, CollisionData& res);

// --------------------------------------------------------------------------
// test collision between a circle and a rectangle
bool Circle2Rect(const CircleEntity& c, const RectEntity& r, CollisionData& res);


#endif // PHYSIC_ENTITY_HPP
