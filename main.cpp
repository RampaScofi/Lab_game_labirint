#ifndef TEST
#include "Model.h"
#include "View.h"
#include "Controller.h"
#include <iostream>

int main() {
    try {
        Model model;
        model.loadLevel("levels/level3.txt");
        View view(model);
        Controller controller(model, view);
        controller.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Îøèáêà: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
#endif