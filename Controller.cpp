#include "Controller.h"

Controller::Controller(Model& model, View& view) : model(model), view(view) {}

void Controller::handleInput() {
    std::vector<Direction> directions = view.getInputDirections();
    for (const auto& direction : directions) {
        int dx = 0, dy = 0;
        switch (direction) {
        case Direction::Up: dy = -1; break;
        case Direction::Down: dy = 1; break;
        case Direction::Left: dx = -1; break;
        case Direction::Right: dx = 1; break;
        case Direction::Close: view.closeWindow(); return;
        }
        if (dx != 0 || dy != 0) {
            model.movePlayer(dx, dy);
        }
    }
}

void Controller::run() {
    while (view.isRunning()) {
        handleInput();
        if (model.isFinished()) {
            view.closeWindow();
        }
        view.render();
    }
}