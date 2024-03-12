#pragma once

#include <SFML/Graphics.hpp>
#include <map>

class World{

    sf::Shader updater;
    sf::VertexArray canvas;
    sf::RenderStates pair_ts;
    sf::RenderTexture *state_a, *state_b;

    sf::Clock clock;
    sf::Time elapsed;
    sf::Time delay;

    std::vector<sf::Color> target_colors;
    
public:
    World(const char* engine, sf::Image& state_0, std::vector<sf::Color>& colors);

    ~World();

    void update();

    sf::Sprite drawable();

    sf::Text getStatsAsText();
};