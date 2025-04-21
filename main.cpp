#include "Model.h"
#include "View.h"
#include "Controller.h"

int main() {
    Model model;
    model.loadLevel("levels/level4.txt");

    View view(model);
    Controller controller(model, view);

    controller.run();

    return 0;
}