#include <SFML/Graphics.hpp>
#include "src/board.hpp"
#include <iostream>
#include <cmath>

sf::Event event;

const uint32_t W = 800;
const uint32_t H = 600;

sf::Color randcolor(){
    int r = rand()%256;
    if (r < 20){
        return sf::Color::White;
    }
    if (r >= 20 && r < 40){
        return sf::Color::Red;
        //return sf::Color::Green;
    }
    if (r >= 40 && r < 60){
        return sf::Color::Yellow;
    }
    if (r >= 60 && r < 80){
        return sf::Color::Cyan;
    }
    if (r >= 80 && r < 100){
        return sf::Color::Blue;
    }
    if (r >= 100 && r < 120){
        return sf::Color::Magenta;
    }
    return sf::Color::Black;
}

int main(){
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(W, H), "Arena Brawl");
    window.setVerticalSyncEnabled(true);

    sf::Image state_0;
    state_0.create(400, 400);

    for (int y = 0; y < state_0.getSize().y; ++y){
        for (int x = 0; x < state_0.getSize().x; ++x){
            state_0.setPixel(x, y, randcolor());
        }
    }

    std::vector<sf::Color> target_colors = {sf::Color::White, sf::Color::Red, sf::Color::Yellow,
                                            sf::Color::Cyan, sf::Color::Blue, sf::Color::Magenta};

    World simulation("auto.frag", state_0, target_colors);

    sf::View camera(sf::FloatRect(0, 0, W, H));
    camera.setCenter(state_0.getSize().x/2, state_0.getSize().y/2);

    while (window.isOpen()){
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)){
            camera.zoom(0.99f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)){
            camera.zoom(1.01f);
        }

        float offset_vel = std::hypot(camera.getSize().x, camera.getSize().y) / 300;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            camera.move(0.f, -offset_vel);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            camera.move(0.f, offset_vel);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            camera.move(-offset_vel, 0.f);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            camera.move(offset_vel, 0.f);
        }

        window.setView(camera);

        simulation.update();

        window.clear(sf::Color::White);
        window.draw(simulation.drawable());
        window.display();
    }

    return 0;
}
