#include "engine/core/GameEngine.h"

#include <filesystem>


int main(int argc, char* argv[])
{
    const std::string& gameName = "BareBonesShooter";
    const std::string& assetsFile = "assets.txt";
    
    // Determine list of scenes (from text file or hardcoded?)
    

    // Instantiate game with name, assets and scenes    
    // Finds executable directory -> finding config file is working directory-agnostic
    std::filesystem::path executableDir = std::filesystem::absolute(argv[0]).parent_path();
    executableDir = std::filesystem::canonical(executableDir); // Resolves . and .. cleanly
    
    GameEngine game(executableDir);
    game.run();

    return 0;
}