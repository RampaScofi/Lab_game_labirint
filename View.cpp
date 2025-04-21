#include "View.h"
#include <stdexcept>

View::View(const Model& model)
    : model(model), window(sf::VideoMode(model.getLevelWidth() * 64, model.getLevelHeight() * 64), "Ћабиринт") {
    if (!textureWall.loadFromFile("assets/wall.jpg")) {
        throw std::runtime_error("ќшибка загрузки текстуры стены");
    }
    if (!textureDoor.loadFromFile("assets/finish.jpg")) {
        throw std::runtime_error("ќшибка загрузки текстуры двери");
    }
    if (!texturePlayer.loadFromFile("assets/character.png")) {
        throw std::runtime_error("ќшибка загрузки текстуры игрока");
    }
    if (!textureApple.loadFromFile("assets/apple.png")) {
        throw std::runtime_error("ќшибка загрузки текстуры €блока");
    }

    spriteWall.setTexture(textureWall);
    spriteDoor.setTexture(textureDoor);
    spritePlayer.setTexture(texturePlayer);
    spriteApple.setTexture(textureApple);

    setSpriteSize(spriteWall, 64, 64);
    setSpriteSize(spriteDoor, 64, 64);
    setSpriteSize(spritePlayer, 64, 64);
    setSpriteSize(spriteApple, 64, 64);
}

void View::render() {
    window.clear();

    // ќтрисовка карты
    for (int y = 0; y < model.getLevelHeight(); ++y) {
        for (int x = 0; x < model.getLevelWidth(); ++x) {
            sf::Sprite sprite;
            if (model.getMap()[y][x] == '#') {
                sprite = spriteWall;
            }
            else if (x == model.getFinishX() && y == model.getFinishY()) {
                sprite = spriteDoor;
            }
            sprite.setPosition(x * 64, y * 64);
            window.draw(sprite);
        }
    }

    // ќтрисовка €блока и игрока
    spriteApple.setPosition(model.getAppleX() * 64, model.getAppleY() * 64);
    window.draw(spriteApple);
    spritePlayer.setPosition(model.getPlayerX() * 64, model.getPlayerY() * 64);
    window.draw(spritePlayer);

    window.display();
}

void View::closeWindow() {
    window.close();
}

bool View::isRunning() const {
    return window.isOpen();
}

std::vector<Direction> View::getInputDirections() {
    std::vector<Direction> directions;
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Up: directions.push_back(Direction::Up); break;
            case sf::Keyboard::Down: directions.push_back(Direction::Down); break;
            case sf::Keyboard::Left: directions.push_back(Direction::Left); break;
            case sf::Keyboard::Right: directions.push_back(Direction::Right); break;
            default: break;
            }
        }
        else if (event.type == sf::Event::Closed) {
            directions.push_back(Direction::Close);
        }
    }
    return directions;
}

void View::setSpriteSize(sf::Sprite& sprite, int width, int height) {
    sprite.setTextureRect(sf::IntRect(0, 0, width, height));
}