#include "Model.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

Model::Model() {}

void Model::loadLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл уровня");
    }

    std::string firstLine;
    std::getline(file, firstLine);
    size_t pos = firstLine.find('x');
    if (pos == std::string::npos) {
        throw std::runtime_error("Неверный формат первой строки файла уровня");
    }

    levelWidth = std::stoi(firstLine.substr(0, pos));
    levelHeight = std::stoi(firstLine.substr(pos + 1));
    map.clear();
    map.resize(levelHeight);

    std::string line;
    int row = 0;
    while (std::getline(file, line)) {
        if (row >= levelHeight) {
            throw std::runtime_error("Слишком много строк в файле уровня");
        }
        if (line.size() < levelWidth) {
            throw std::runtime_error("Недостаточно символов в строке уровня");
        }
        map[row] = line;
        for (int col = 0; col < levelWidth; ++col) {
            if (line[col] == 'A') {
                playerX = col;
                playerY = row;
                map[row][col] = ' ';
            }
            else if (line[col] == 'G') {
                appleX = col;
                appleY = row;
                map[row][col] = ' ';
            }
            else if (line[col] == 'F') {
                finishX = col;
                finishY = row;
                map[row][col] = ' ';
            }
        }
        ++row;
    }

    if (row != levelHeight) {
        throw std::runtime_error("Недостаточно строк в файле уровня");
    }
}

bool Model::movePlayer(int dx, int dy) {
    int newX = playerX + dx;
    int newY = playerY + dy;

    // Проверка границ и стен
    if (newX < 0 || newX >= levelWidth || newY < 0 || newY >= levelHeight || map[newY][newX] == '#') {
        return false;
    }

    // Проверка яблока
    if (newX == appleX && newY == appleY) {
        int appleNewX = appleX + dx;
        int appleNewY = appleY + dy;
        if (appleNewX < 0 || appleNewX >= levelWidth || appleNewY < 0 || appleNewY >= levelHeight || map[appleNewY][appleNewX] == '#') {
            return false;
        }
        appleX = appleNewX;
        appleY = appleNewY;
    }

    playerX = newX;
    playerY = newY;
    return true;
}

bool Model::isFinished() const {
    return playerX == finishX && playerY == finishY;
}