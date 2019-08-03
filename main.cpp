
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physic_engine.hpp"

struct EntityRenderer
{
    sf::RectangleShape sf_rect;
    sf::CircleShape sf_circle;
    sf::RenderWindow* sf_window;
    
    EntityRenderer(sf::RenderWindow* window)
        : sf_window(window)
    {}
    
    void drawRect(const Vec2& position, float rotation, float width, float height)
    {
        Vec2 size(width,height);
        sf_rect.setPosition( position );
        sf_rect.setRotation( rotation );
        sf_rect.setSize(size);
        sf_rect.setFillColor(sf::Color::Transparent);
        sf_rect.setOrigin(size * 0.5f);
        sf_rect.setOutlineThickness(1.0);
        sf_window->draw(sf_rect);
    }
    
    void drawCircle(const Vec2& position, float rotation, float radius)
    {
        sf_circle.setRadius(radius);
        sf_circle.setOrigin(Vec2(radius,radius));
        sf_circle.setFillColor(sf::Color::Transparent);
        sf_circle.setOutlineThickness(1.0);
        sf_circle.setPosition( position );
        sf_window->draw(sf_circle);
        
        // additionnal line for seeing rotation
        sf_rect.setSize( Vec2(radius,1.f) );
        sf_rect.setFillColor(sf::Color::White);
        sf_rect.setOutlineThickness(0.0);
        sf_rect.setOrigin( Vec2(0.f,0.f) );
        sf_rect.setPosition(position);
        sf_rect.setRotation(rotation);
        sf_window->draw(sf_rect);
    }
    
    void draw(const Entity* e)
    {
        const GroupEntity* ge = dynamic_cast<const GroupEntity*>(e);
        const RectEntity* re = dynamic_cast<const RectEntity*>(e);
        const CircleEntity* ce = dynamic_cast<const CircleEntity*>(e);
        
        if(ge)
            for(auto& e2 : ge->entities) draw(e2);
        if(re)
            drawRect(re->position,re->rotation,re->width,re->height);
        if(ce)
            drawCircle(ce->position,ce->rotation,ce->radius);
    }
};

int main(int argc, char* argv[])
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(512, 512), "Skeletal-Test");
    EntityRenderer renderer(&window);
    
    PhysicEngine phyEngine;
    phyEngine.addEntity( new RectEntity(Vec2(230.f, 250.f),50.f,50.f) );
    phyEngine.addEntity( new RectEntity(Vec2(240.f, 200.f),30.f,30.f) );
    
    phyEngine.addEntity( new RectEntity(Vec2(200.f, 195.f),20.f,50.f) );
    phyEngine.addEntity( new RectEntity(Vec2(250.f, 190.f),24.f,14.f) );
    phyEngine.addEntity( new RectEntity(Vec2(150.f, 110.f),18.f,33.f) );
    phyEngine.addEntity( new RectEntity(Vec2(300.f, 150.f),54.f,108.f) );
    
    
    phyEngine.addEntity( new CircleEntity(Vec2(204.f, 115.f),5.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(200.f, 100.f),10.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(198.f, 105.f),7.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(196.f, 90.f),8.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(199.f, 55.f),14.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(203.f, 70.f),12.f) );
    
    phyEngine.addEntity( new BoxEntity(450.f, 450.f, 30.f, Vec2(250.f,250.f), 0.f) );
    
    sf::Clock clock;
    
    // run the main loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
            if(event.type == sf::Event::Closed) window.close();
        
        // update
        float elapsed_ms = clock.getElapsedTime().asMilliseconds();
        if(elapsed_ms >= 1.0)
        {
            float elapsed_sec = elapsed_ms * 0.001;
            phyEngine.updateEntities( elapsed_sec );
            clock.restart();
        }
        
        // draw
        window.clear();
        for(auto& e : phyEngine.entities) renderer.draw(e);
        window.display();
    }

    return 0;
}
