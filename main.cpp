
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physics/physic_engine.hpp"
#include "renderer.hpp"

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    if(argc>1) args = std::vector<std::string>(argv+1,argv+argc);

    // create the window
    sf::RenderWindow window(sf::VideoMode(512, 512), "PhysicEngine2D_Test");
    EntityRenderer renderer(&window);
    
    PhysicEngine phyEngine;

    phyEngine.addEntity( new RectEntity(Vec2(230.f, 250.f),50.f,50.f) );
    phyEngine.addEntity( new RectEntity(Vec2(240.f, 200.f),30.f,30.f) );

    phyEngine.addEntity( new RectEntity(Vec2(200.f, 295.f),20.f,50.f) );
    phyEngine.addEntity( new RectEntity(Vec2(250.f, 310.f),24.f,14.f) );
    phyEngine.addEntity( new RectEntity(Vec2(210.f, 320.f),18.f,33.f) );
    phyEngine.addEntity( new RectEntity(Vec2(300.f, 280.f),54.f,108.f) );
    phyEngine.addEntity( new RectEntity(Vec2(330.f, 450.f),14.f,24.f) );

    phyEngine.addEntity( new RectEntity(Vec2(305.f, 400.f),18.f,32.f) );
    phyEngine.addEntity( new RectEntity(Vec2(280.f, 410.f),20.f,50.f) );
    phyEngine.addEntity( new RectEntity(Vec2(170.f, 340.f),24.f,14.f) );
    phyEngine.addEntity( new RectEntity(Vec2(190.f, 360.f),18.f,33.f) );
    phyEngine.addEntity( new RectEntity(Vec2(310.f, 350.f),41.f,10.f) );


    phyEngine.addEntity( new CircleEntity(Vec2(204.f, 115.f),5.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(200.f, 100.f),10.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(198.f, 105.f),7.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(196.f, 90.f),8.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(199.f, 55.f),14.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(203.f, 70.f),12.f) );

    phyEngine.addEntity( new CircleEntity(Vec2(304.f, 215.f),5.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(300.f, 200.f),10.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(298.f, 205.f),7.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(296.f, 190.f),8.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(299.f, 155.f),14.f) );
    phyEngine.addEntity( new CircleEntity(Vec2(203.f, 170.f),12.f) );


    phyEngine.addEntity( new BoxEntity(450.f, 450.f, 30.f, Vec2(250.f,250.f), 0.f) );
    
    sf::Clock clock;

    // first init
    float elapsed_ms = clock.getElapsedTime().asMilliseconds();
    phyEngine.updateEntities( elapsed_ms * 0.001 );

    bool started = false;
    
    // run the main loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) window.close();
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) started = true;
        }
        
        // update
        elapsed_ms = clock.getElapsedTime().asMilliseconds();
        if(elapsed_ms >= 0.1)
        {
            float elapsed_sec = elapsed_ms * 0.001;
            if(started) phyEngine.updateEntities( elapsed_sec );
            clock.restart();
        }
        
        // draw
        window.clear();
        for(auto& e : phyEngine.entities) renderer.draw(e);
        for(auto& c : phyEngine.collisions) renderer.draw(c);

        window.display();
    }

    return 0;
}
