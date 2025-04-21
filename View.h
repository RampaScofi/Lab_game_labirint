#pragma once
#include "Model.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum class Direction {
    Up,
    Down,
    Left,
    Right,
    Close
};

class View {
public:
    View(const Model& model);
    void render();
    void closeWindow();
    bool isRunning() const;
    std::vector<Direction> getInputDirections();

private:
    const Model& model;
    sf::RenderWindow window;
    sf::Texture textureWall, textureDoor, texturePlayer, textureApple;
    sf::Sprite spriteWall, spriteDoor, spritePlayer, spriteApple;
    void setSpriteSize(sf::Sprite& sprite, int width, int height);
};