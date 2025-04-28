#pragma once
#include <string>
#include <vector>

class Model {
public:
    Model();
    void loadLevel(const std::string& filename);
    bool movePlayer(int dx, int dy);
    bool isFinished() const;
    int getPlayerX() const { return playerX; }
    int getPlayerY() const { return playerY; }
    int getAppleX() const { return appleX; }
    int getAppleY() const { return appleY; }
    int getFinishX() const { return finishX; }
    int getFinishY() const { return finishY; }
    int getLevelWidth() const { return levelWidth; }
    int getLevelHeight() const { return levelHeight; }
    const std::vector<std::string>& getMap() const { return map; }

private:
    void logError(const std::string& message) const;
    std::vector<std::string> map;
    int levelWidth = 0, levelHeight = 0;
    int playerX = 0, playerY = 0;
    int appleX = 0, appleY = 0;
    int finishX = 0, finishY = 0;
};