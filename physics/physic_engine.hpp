#ifndef PHYSIC_ENGINE_HPP
#define PHYSIC_ENGINE_HPP

#include "physic_entity.hpp"


// --------------------------------------------------------------------------
// Main interface for physic entity animating
struct PhysicEngine
{
    // list of registered entitites
    Arr<Entity*> entities;
    
    // detected collision list
    Arr<CollisionData> collisions;
    
    // gravity direction and force
    Vec2 gravityVec;
    float gravityForce;
    
    PhysicEngine();
    virtual ~PhysicEngine();
    
    // register an entity
    void addEntity(Entity* e);
    
    // update all registered entities
    void updateEntities(float elapsedSec);

    // collisions detection and resolving
    void collectCollisions();
    void resolveCollision(CollisionData& collision);
    void resolveCollisions(float elapsedSec);
    
    // cancel penetration distance between 2 entitties
    void resolvePenetration(CollisionData& mf);

    // apply gravity and collisions effects
    void applyGravity(float elapsedSec);
    void applyResponse(Entity& e1, const Vec2& contact, const Vec2& normal2,Entity& e2);

    // appply linear and angular velocities on position and rotation
    void advanceTransformation(float elapsedSec);
};


#endif // PHYSIC_ENGINE_HPP
