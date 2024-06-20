#include "GameEngine.h"

int main()
{
    GameEngine game("/bin/config.txt");
    game.run();

    return 0;
}