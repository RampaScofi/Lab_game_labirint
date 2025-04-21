#pragma once
#include "Model.h"
#include "View.h"

class Controller {
public:
    Controller(Model& model, View& view);
    void run();

private:
    void handleInput();
    Model& model;
    View& view;
};