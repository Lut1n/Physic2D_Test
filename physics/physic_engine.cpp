#include "physic_engine.hpp"
#include <cmath>
#include <iostream>

#define GRAVITY 9.80665
#define PIXEL_PER_METER 2

// --------------------------------------------------------------------------
PhysicEngine::PhysicEngine()
{
    
    const float SPEED_FACTOR = 1.0/PIXEL_PER_METER;
    gravityVec = Vec2(0.f,1.f);
    gravityForce = GRAVITY * SPEED_FACTOR;
}

// --------------------------------------------------------------------------
PhysicEngine::~PhysicEngine()
{
}

// --------------------------------------------------------------------------
void PhysicEngine::addEntity(Entity* e)
{
    entities.push_back(e);
}

// --------------------------------------------------------------------------
void PhysicEngine::updateEntities(float elapsedSec)
{
    applyGravity(elapsedSec);
    
    collectCollisions();
    resolveCollisions(elapsedSec);
    advanceTransformation(elapsedSec);
}

// --------------------------------------------------------------------------
void PhysicEngine::resolvePenetration(CollisionData& collision)
{
    Entity& e1  = *collision.e1;
    Entity& e2  = *collision.e2;

    float massTT = e1.mass + e2.mass;
    if(massTT == 0.f) return;
    
    float ratio1 = e1.mass / massTT;
    float ratio2 = e2.mass / massTT;

    const float EPSILON = 0.02;
    float correction = (float)collision.penetration * (1.0+EPSILON);

    e1.position += collision.normal2 * correction * ratio1;
    e2.position += collision.normal1 * correction * ratio2;
}

// --------------------------------------------------------------------------
void PhysicEngine::applyResponse(Entity& e1, const Vec2& hitPoint, const Vec2& normal2,Entity& e2)
{
    sf::Transform rot90; rot90.rotate(90.0);
    Vec2 fromO = hitPoint - e1.position;
    fromO = normalize(fromO);
    Vec2 tan = rot90 * fromO;

    Vec2 surface2 = rot90 * -normal2;

    if(e1.mass != 0.f)
    {
        float hitPoint_dist = len( hitPoint - e1.position );
        float tanVel_value = std::tan(e1.v_angular * 3.1415f / 180.f);
        Vec2 tanVel = tan * tanVel_value * hitPoint_dist;
        Vec2 velocityAtContact = e1.v_linear + tanVel;		
        
        // velocity
        float impulse = std::max( -dot(normal2,velocityAtContact) , 0.0f);
        const float friction = 0.7;
        float slow = dot(surface2,velocityAtContact);
        float response = 1.0;
        if(e2.mass != 0.f)
        {
            response = e1.mass / (e1.mass + e2.mass);
        }
        float restitution = e1.restitution;
        Vec2 impulseVec = normal2 * impulse * (float)(response + restitution);
        impulseVec -= surface2 * slow * (float)(friction);
        
        // linear velocity
        e1.v_linear += fromO*dot(fromO, impulseVec);
        
        // angular velocity
        float tanImpulse = dot(impulseVec,tan);
        float angularImpulse = std::atan(tanImpulse / hitPoint_dist) * 180.0 / 3.1415;
        angularImpulse -= e1.v_angular;// * (response + restitution);
        e1.v_angular += angularImpulse;
    }
}

// --------------------------------------------------------------------------
void PhysicEngine::resolveCollision(CollisionData& collision)
{
    Entity& e1 = *collision.e1;
    Entity& e2 = *collision.e2;

    resolvePenetration(collision);

    applyResponse(e1, collision.hitPoint, collision.normal2,e2);
    applyResponse(e2, collision.hitPoint, collision.normal1,e1);
}

// --------------------------------------------------------------------------
void PhysicEngine::applyGravity(float elapsedSec)
{
    for(auto& e:entities)
    {
        if(e->mass != 0.f)
            e->v_linear += gravityVec * gravityForce * elapsedSec;
    }
}

// --------------------------------------------------------------------------
void PhysicEngine::collectCollisions()
{
    collisions.clear();

    Arr< std::pair<Entity*,Entity*> > pairs;
    for(auto& e1 : entities)
    {
        if( dynamic_cast<GroupEntity*>(e1) ) continue;
            
        for(auto& e2 : entities)
        {
            auto p = std::make_pair(e1,e2);
            auto it = std::find(pairs.begin(),pairs.end(),p);
            if( e1==e2 || it!=pairs.end() ) continue;
            
            CollisionData res_coll;
            
            if( Entity2Entity(*e1,*e2,res_coll) ) 
            {
                collisions.push_back(res_coll);
                pairs.push_back( p );
            }
        }
    }
}

// --------------------------------------------------------------------------
void PhysicEngine::resolveCollisions(float elapsedSec)
{
    for(auto& coll : collisions)
        resolveCollision(coll);
}

// --------------------------------------------------------------------------
void advance(Entity* e)
{
    if(e->mass != 0.f)
    {
        e->position += e->v_linear;
        e->rotation += e->v_angular;
    
        if(e->v_linear.x > 0.0001) e->v_linear.x-=0.0001;
        else if(e->v_linear.x < -0.0001) e->v_linear.x+=0.0001;
        else e->v_linear.x=0.0;
        
        if(e->v_angular > 0.001) e->v_angular-=0.001;
        else if(e->v_angular < -0.001) e->v_angular+=0.001;
        else e->v_angular=0.0;
    }
    
    RectEntity* re = dynamic_cast<RectEntity*>(e);
    if(re) { re->change(); re->update(); }
    
    CircleEntity* ce = dynamic_cast<CircleEntity*>(e);
    if(ce) { ce->center=ce->position; }
}

// --------------------------------------------------------------------------
void PhysicEngine::advanceTransformation(float elapsedSec)
{
    for(auto& e : entities)
    {
        GroupEntity* ge = dynamic_cast<GroupEntity*>(e);
        if(ge)
        {
            for(auto& e2 : ge->entities) advance(e2);
        }
        else
        {
            advance(e);
        }
    }
}
