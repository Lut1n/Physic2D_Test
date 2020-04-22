
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physics/physic_entity.hpp"

class EntityRenderer
{
public:
    EntityRenderer(sf::RenderWindow* window);

    void draw(const Entity* e, const sf::Color& color = sf::Color(50,50,128));
    void draw(const CollisionData& collision);

    void drawRect(const Vec2& position, float rotation, float width, float height, const sf::Color& color = sf::Color(50,50,128));
    void drawCircle(const Vec2& position, float rotation, float radius);
    void drawPoint(const Vec2& position);

protected:
    sf::RectangleShape sf_rect;
    sf::CircleShape sf_circle;
    sf::RenderWindow* sf_window;
};
