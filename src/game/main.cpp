#include "engine/core/GameEngine.h"

#include <filesystem>


int main(int argc, char* argv[])
{
    const std::string& gameName = "BareBonesShooter";

    std::filesystem::path executableDir = std::filesystem::absolute(argv[0]).parent_path(); // Necessary so finding assets and config files is working directory-agnostic
    executableDir = std::filesystem::canonical(executableDir);                              // Resolves . and .. cleanly
    
    GameEngine game(executableDir, gameName);
    game.run();

    return 0;
}