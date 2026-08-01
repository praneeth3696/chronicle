#include "console.hpp"
#include <string>

int main(int argc, char* argv[]) {
    std::string world_file = "";
    if (argc > 1) {
        world_file = argv[1];
    }

    console(world_file);
    return 0;
}