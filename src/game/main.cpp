#include "engine/core/GameEngine.h"

#include <filesystem>

int main(int argc, char* argv[])
{
    // Finds executable directory -> finding config and level files is working directory-agnostic
    std::filesystem::path executableDir = std::filesystem::absolute(argv[0]).parent_path();
    executableDir = std::filesystem::canonical(executableDir); // Resolves . and .. cleanly
    
    GameEngine game(executableDir);
    game.run();

    return 0;
}