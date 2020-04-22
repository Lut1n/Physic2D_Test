
#include "renderer.hpp"

// --------------------------------------------------------------------------
EntityRenderer::EntityRenderer(sf::RenderWindow* window)
    : sf_window(window)
{}

// --------------------------------------------------------------------------
void EntityRenderer::drawRect(const Vec2& position, float rotation, float width, float height, const sf::Color& color)
{
    Vec2 size(width,height);
    sf_rect.setPosition( position );
    sf_rect.setRotation( rotation );
    sf_rect.setSize(size);
    sf_rect.setFillColor(color);
    sf_rect.setOrigin(size * 0.5f);
    sf_rect.setOutlineThickness(2.0);
    sf_window->draw(sf_rect);
}

// --------------------------------------------------------------------------
void EntityRenderer::drawCircle(const Vec2& position, float rotation, float radius)
{
    sf_circle.setRadius(radius);
    sf_circle.setOrigin(Vec2(radius,radius));
    sf_circle.setFillColor(sf::Color(128,50,50));
    sf_circle.setOutlineThickness(2.0);
    sf_circle.setPosition( position );
    sf_window->draw(sf_circle);

    // additionnal line for seeing rotation
    sf_rect.setSize( Vec2(radius,1.f) );
    sf_rect.setFillColor(sf::Color::White);
    sf_rect.setOutlineThickness(1.0);
    sf_rect.setOrigin( Vec2(0.f,0.f) );
    sf_rect.setPosition(position);
    sf_rect.setRotation(rotation);
    sf_window->draw(sf_rect);
}

// --------------------------------------------------------------------------
void EntityRenderer::drawPoint(const Vec2& position)
{
    Vec2 size(4,4);
    sf_rect.setPosition( position );
    sf_rect.setRotation( 0.0 );
    sf_rect.setSize(size);
    sf_rect.setFillColor(sf::Color::Red);
    sf_rect.setOrigin(size * 0.5f);
    sf_rect.setOutlineThickness(0.0);
    sf_window->draw(sf_rect);
}

// --------------------------------------------------------------------------
void EntityRenderer::draw(const Entity* e, const sf::Color& color)
{
    const GroupEntity* ge = dynamic_cast<const GroupEntity*>(e);
    const RectEntity* re = dynamic_cast<const RectEntity*>(e);
    const CircleEntity* ce = dynamic_cast<const CircleEntity*>(e);

    if(ge)
        for(auto& e2 : ge->entities) draw(e2, sf::Color(70,70,70));
    if(re)
        drawRect(re->position,re->rotation,re->width,re->height, color);
    if(ce)
        drawCircle(ce->position,ce->rotation,ce->radius);
}

// --------------------------------------------------------------------------
void EntityRenderer::draw(const CollisionData& collision)
{
    drawPoint(collision.hitPoint);
}
