#include "include/controller/grep_controller.hpp"

int main(int argc, char* argv[]) {
    controller::grep_controller controller(argc, argv);
    return controller.run();
}
