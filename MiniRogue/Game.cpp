
#include "Game.h"
#include "utilities.h"
#include "Dungeon.h"
#include "Actors.h"
#include <iostream>
#include <cstdlib>
using namespace std;

//Constructor/Destructor////////////////////////////////////////////////////////////////////////////////////////
Game::Game(int goblinSmellDistance){
    if (goblinSmellDistance < 1) {
        cout << "***** Cannot create Game with no goblin smell distance!" << endl;
        exit(1);
    }
    ///Creates Dungeon
    m_dungeon = new Dungeon(0, goblinSmellDistance);
    ///Adds player
    m_dungeon->addPlayer();
    ///Adds monsters
    m_dungeon->addMonsters();
}

Game::~Game(){
    delete m_dungeon;
}

//Mutator Functions////////////////////////////////////////////////////////////////////////////////////////
void Game::play(){
    cout << "Press q to exit game." << endl;
    m_dungeon->display("");
    Player* player = m_dungeon->player();
    char input = getCharacter();
    while (input != 'q' && !player->wonGame()){
        ///Takes player's turn
        string msg = player->move(input);
        if(msg == "CHANGELEVEL"){
            generateNewLevel();
            input = '~';    ///In the off chance the player is placed onto a stairway and teleports again, as continue will go to top of while statement
            continue;
        }
        if(msg == "blocked")
            msg = "";
        if(player->wonGame()){
            break;
        }
        ///Takes monsters turn
        for(int i = 0; i < m_dungeon->monsterSize(); i++){
            string m = m_dungeon->monster(i)->move();
            if(m != "")
                msg += m;
        }
        m_dungeon->display(msg);
        ///checks if player is dead
        if(player->isDead())
            break;
        input = getCharacter();
    }
    if(player->wonGame()){
        cout << "You pick up the golden idol" << endl;
        cout << "Congratulations, you won!" << endl;
        cout << "Press q to exit game." << endl;
        while(getCharacter()!= 'q');
    }
    if(player->isDead()){
        cout << "Press q to exit game." << endl;
        while(getCharacter()!= 'q');
    }
}

void Game::generateNewLevel(){
    m_dungeon->increaseLevel();
    m_dungeon->generateDungeon();
    ///add Player
    m_dungeon->movePlayer();
    ///add monsters!
    m_dungeon->deleteAllMonsters();
    m_dungeon->addMonsters();
}
