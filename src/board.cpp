#include "board.hpp"
//#include <cuda.h>
//#include <cuda_runtime.h>

World::World(const char* engine, sf::Image& state_0, std::vector<sf::Color>& colors){
    int W = state_0.getSize().x;
    int H = state_0.getSize().y;

    canvas.setPrimitiveType(sf::Quads);
    canvas.resize(4);
    canvas[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::White, sf::Vector2f(0, 0));
    canvas[1] = sf::Vertex(sf::Vector2f(0, H), sf::Color::White, sf::Vector2f(0, H));
    canvas[2] = sf::Vertex(sf::Vector2f(W, H), sf::Color::White, sf::Vector2f(W, H));
    canvas[3] = sf::Vertex(sf::Vector2f(W, 0), sf::Color::White, sf::Vector2f(W, 0));

    sf::Texture gen_0;
    gen_0.loadFromImage(state_0);

    target_colors.assign(colors.begin(), colors.end());

    state_a = new sf::RenderTexture;
    state_a->create(W, H, sf::ContextSettings());
    state_a->draw(canvas, &gen_0);
    state_a->display();

    state_b = new sf::RenderTexture;
    state_b->create(W, H, sf::ContextSettings());
    state_b->display();

    updater.loadFromFile(engine, sf::Shader::Fragment);
    updater.setUniform("resolution", sf::Vector2f(W, H));
    updater.setUniform("void_color", sf::Vector3f(0.f, 0.f, 0.f));
    updater.setUniform("virus_color", sf::Vector3f(0.f, 1.f, 0.f));

    pair_ts.shader = &updater;
    pair_ts.texture = &state_a->getTexture();

    delay = sf::seconds(0.05);
}

World::~World(){
    delete state_a;
    delete state_b;
}

void World::update(){
    if (elapsed >= delay){
        /*
        * Draw something in state_a
        */
        pair_ts.texture = &state_a->getTexture();

        updater.setUniform("generation", state_a->getTexture());

        state_b->clear(sf::Color::Black);
        state_b->draw(canvas, pair_ts);
        state_b->display();
        /*
        sf::Image out = state_b->getTexture().copyToImage();
        std::map<int, int> stats;

        for (int y = 0; y < out.getSize().y; ++y){
            for (int x = 0; x < out.getSize().x; ++x){
                int pixel = out.getPixel(x, y).toInteger();
                if (stats.count(pixel))
                    ++stats[pixel];
                else
                    stats[pixel] = 1;
            }
        }*/

        std::swap(state_a, state_b);
        elapsed = sf::Time::Zero;
    }
    elapsed += clock.restart();
}

sf::Sprite World::drawable(){
    return sf::Sprite(state_a->getTexture());
}

sf::Text World::getStatsAsText(){
    return sf::Text();
}