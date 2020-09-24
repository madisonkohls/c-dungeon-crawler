// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <string>

class Dungeon;

class Game{
public:
    //Constructor/Destructor
    Game(int goblinSmellDistance);
    ~Game();
    //Mutator Functions
    void play();
    void generateNewLevel();
private:
    Dungeon* m_dungeon;
};

#endif // GAME_INCLUDED
