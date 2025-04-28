#include "Model.h"
#include <fstream>
#include <sstream>
#include <ctime>

Model::Model() : levelWidth(0), levelHeight(0), playerX(0), playerY(0), appleX(0), appleY(0), finishX(0), finishY(0) {}

void Model::logError(const std::string& message) const {
    std::ofstream logFile("game.log", std::ios::app);
    if (logFile.is_open()) {
        std::time_t now = std::time(nullptr);
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &now);
        std::string timeStrClean(timeStr);
        if (!timeStrClean.empty() && timeStrClean.back() == '\n') {
            timeStrClean.pop_back();
        }
        logFile << "[" << timeStrClean << "] " << message << std::endl;
        logFile.close();
    }
}

void Model::loadLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        logError("�� ������� ������� ����: " + filename);
        throw std::runtime_error("�� ������� ������� ����");
    }

    std::string line;
    if (!std::getline(file, line) || line.empty()) {
        file.close();
        logError("���� ����: " + filename);
        throw std::runtime_error("���� ����");
    }

    std::istringstream iss(line);
    int width, height;
    char x;
    if (!(iss >> width >> x >> height) || x != 'x' || width <= 0 || height <= 0) {
        file.close();
        if (x != 'x') {
            logError("�������� ������ ������ ������: " + line);
            throw std::runtime_error("�������� ������ ������ ������");
        }
        logError("������������ �������: " + line);
        throw std::runtime_error("������������ �������");
    }

    map.clear();
    int playerCount = 0;
    for (int y = 0; y < height; ++y) {
        if (!std::getline(file, line)) {
            file.close();
            logError("������������ ����� � �����: " + filename);
            throw std::runtime_error("������������ �����");
        }
        if (line.length() < static_cast<size_t>(width)) {
            file.close();
            logError("������������ �������� � ������ " + std::to_string(y + 1));
            throw std::runtime_error("������������ �������� � ������ " + std::to_string(y + 1));
        }
        map.push_back(line);
        for (int x = 0; x < width; ++x) {
            char cell = line[x];
            if (cell == 'A') {
                playerX = x;
                playerY = y;
                playerCount++;
            }
            else if (cell == 'G') {
                appleX = x;
                appleY = y;
            }
            else if (cell == 'F') {
                finishX = x;
                finishY = y;
            }
            else if (cell != ' ' && cell != '#') {
                file.close();
                logError("������������ ������ � ������ " + std::to_string(y + 1) + ": " + cell);
                throw std::runtime_error("������������ ������");
            }
        }
    }

    if (playerCount == 0) {
        file.close();
        logError("����� �� ������ � �����: " + filename);
        throw std::runtime_error("����� �� ������");
    }
    if (playerCount > 1) {
        file.close();
        logError("���������� ��������� ������� � �����: " + filename);
        throw std::runtime_error("���������� ��������� �������");
    }

    file.close();
    levelWidth = width;
    levelHeight = height;
}

bool Model::movePlayer(int dx, int dy) {
    int newX = playerX + dx;
    int newY = playerY + dy;

    if (newX < 0 || newX >= levelWidth || newY < 0 || newY >= levelHeight || map[newY][newX] == '#') {
        return false;
    }

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