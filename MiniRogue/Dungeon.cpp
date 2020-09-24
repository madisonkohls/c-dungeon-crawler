
#include "Dungeon.h"
#include "Actors.h"
#include "utilities.h"
#include <iostream>

using namespace std;

//Constructor/Destructor/////////////////////////////////////////////////////////////////////////////////////
Dungeon::Dungeon(int level, int gobSmell) : m_level(level), m_player(nullptr), m_goblinSmell(gobSmell){
    generateDungeon();
}

Dungeon::~Dungeon(){
    delete m_player;
    deleteAllMonsters();
}

//Accessor Functions////////////////////////////////////////////////////////////////////////////////////////
int Dungeon::level() const{
    return m_level;
}

Player* Dungeon::player() const{
    return m_player;
}

int Dungeon::goblinSmell() const{
    return m_goblinSmell;
}

bool Dungeon::isWall(const int& r, const int& c) const{
    if(m_grid[r][c] == '#')
        return true;
    return false;
}

char Dungeon::actorType(const int& r, const int& c) const{
    return m_actors[r][c];
}

char Dungeon::location(const int& r, const int& c) const{
    return m_grid[r][c];
}

int Dungeon::objectType(const int& r, const int& c) const{
    return m_objects[r][c];
}

Monster* Dungeon::monster(int index) const{
    return m_monsters[index];
}

int Dungeon::monsterSize() const{
    int s = 0;
    for(size_t k = 0; k < m_monsters.size(); k++)
        s++;
    return s;
}

int Dungeon::monsterIndex(const int& r, const int& c) const{
    int index;
    for(index = 0; index < m_monsters.size(); index++){
        if(m_monsters[index]->row() == r && m_monsters[index]->col() == c)
            return index;
    }
    return -1;  ///if there is no such monster
}

void Dungeon::display(string msg) const{
    ///Fills displayGrid with current information
    char displayGrid[MAXROWS][MAXCOLS];
    for(int r = 0; r < MAXROWS; r++){
        for(int c = 0; c < MAXCOLS; c++){
            displayGrid[r][c] = m_grid[r][c];
        }
    }
    ///Adds Player
    if(m_player != nullptr){
        displayGrid[m_player->row()][m_player->col()] = '@';
    }
    ///Adds Monsters
    for(int i = 0; i < m_monsters.size(); i++){
        Goblin* gp = dynamic_cast<Goblin*>(m_monsters[i]);
        if(gp!=nullptr){
            displayGrid[gp->row()][gp->col()] = 'G';
            continue;
        }
        Snakewoman* sp = dynamic_cast<Snakewoman*>(m_monsters[i]);
        if(sp!=nullptr){
            displayGrid[sp->row()][sp->col()] = 'S';
            continue;
        }
        Bogeyman* bp = dynamic_cast<Bogeyman*>(m_monsters[i]);
        if(bp!=nullptr){
            displayGrid[bp->row()][bp->col()] = 'B';
            continue;
        }
        Dragon* dp = dynamic_cast<Dragon*>(m_monsters[i]);
        if(dp!=nullptr){
            displayGrid[dp->row()][dp->col()] = 'D';
            continue;
        }
     }
    ///Writes the current dungeon
    clearScreen();
    for(int r = 0; r < MAXROWS; r++){
        for(int c = 0; c < MAXCOLS; c++){
            cout << displayGrid[r][c];
        }
        cout << endl;
    }
    ///for testing screen
    if(m_player == nullptr)
        return;
    ///Prints out statistics
    cout << "Dungeon Level: " << m_level << ", Hit points: " << m_player->hp() << ", Armor: " << m_player->armor() << ", Strength: " << m_player->strength() << ", Dexterity: " << m_player->dexterity() << endl;
    cout << endl;
    ///Prints out message if applicable
    if(msg != "")
        cout << msg << endl;
}

//Mutator Functions///////////////////////////////////////////////////////////////////////////////////////
void Dungeon::setActor(const int& r, const int& c, const char& ch){
    m_actors[r][c] = ch;
}

void Dungeon::increaseLevel(){
    m_level++;
}

void Dungeon::movePlayer(){
    int r, c;
    do{
        r = randInt(3,15);
        c = randInt(6,60);
    }while(isWall(r, c) || m_actors[r][c] != ' ');
    m_player->teleport(r,c);
}

void Dungeon::addPlayer(){
    int r, c;
    do{
        r = randInt(3,15);
        c = randInt(6,60);
    }while(isWall(r, c) || m_actors[r][c] != ' ');
    if(m_player!=nullptr)   ///don't add a player if there already is one
        return;
    ///creates new player
    m_player = new Player(this, r, c);
    m_actors[r][c] = '@';
}

void Dungeon::setLocation(const int& r, const int& c, int type){    ///updates object grid and displayed grid
    m_objects[r][c] = type;
    switch(type){
        case 0:
            m_grid[r][c] = ' ';
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            m_grid[r][c] = ')';
            break;
        default:
            m_grid[r][c] = '?';
            break;
    }
}

void Dungeon::addMonsters(){
    int nMonsters = randInt(2,5*(m_level+1)+1);
    for(int i = 0; i < nMonsters; i++){
        int r, c;                   ///generates coordinates for monster
        do {
            r = randInt(3,15);
            c = randInt(6,60);
        } while(isWall(r, c) || m_actors[r][c] != ' ');
        int type;
        if(m_level < 2)             ///types of monsters on level 0-1
            type = randInt(1,2);
        else if(m_level < 3)        ///types of monsters on level 0-2
            type = randInt(1,3);
        else                        ///types of monsters on level 0-4
            type = randInt(1,4);
        switch(type){
            case 1: ///Goblin
                m_monsters.push_back(new Goblin(this, r, c, m_goblinSmell));
                m_actors[r][c] = 'g';
                break;
            case 2: ///Snakewoman
                m_monsters.push_back(new Snakewoman(this, r, c));
                m_actors[r][c] = 's';
                break;
            case 3: ///Bogeyman
                m_monsters.push_back(new Bogeyman(this, r, c));
                m_actors[r][c] = 'b';
                break;
            case 4: ///Dragon
                m_monsters.push_back(new Dragon(this, r, c));
                m_actors[r][c] = 'd';
                break;
        }
    }
}

void Dungeon::deleteMonster(const int& i){
    vector<Monster*>::iterator it = m_monsters.begin();
    for(int k = 0; k < i; k++)
        it++;
    m_actors[m_monsters[i]->row()][m_monsters[i]->col()] = ' ';
    delete m_monsters[i];
    m_monsters.erase(it);
}

void Dungeon::deleteAllMonsters(){
    while(m_monsters.size() != 0){
        delete m_monsters[m_monsters.size()-1];
        m_monsters.pop_back();
    }
}

void Dungeon::generateDungeon(){
    ///Creates a walled grid and empty object grid
    for(int r = 0; r < MAXROWS; r++){
        for(int c = 0; c < MAXCOLS; c++){
            m_grid[r][c] = '#';
            m_objects[r][c] = 0;
            m_actors[r][c] = ' ';
        }
    }
    ///Fills grid with rooms
    int lastSpaceCRow = 0, lastSpaceCCol = 0;
    for(int i = 0; i < randInt(4,7); i++){      ///creates 4 to 7 rooms
        room space;
        do{
            space.length = randInt(8, 15);      ///the length and width of the rooms
            space.width = randInt(4,9);
            space.row = randInt(1,MAXROWS-1-space.width);
            space.col = randInt(1, MAXCOLS-1-space.length);
        }while(overlap(space));
        for(int r = space.row; r < space.row + space.width; r++){
            for(int c = space.col; c < space.col + space.length; c++){
                m_grid[r][c] = ' ';
            }
        }
        int curCRow = space.row + space.width/2, curCCol = space.col + space.length/2;
        if(lastSpaceCRow != 0 && lastSpaceCCol != 0)
            connectRooms(curCRow, curCCol, lastSpaceCRow, lastSpaceCCol);
        lastSpaceCRow = curCRow;
        lastSpaceCCol = curCCol;
    }
       ///Adds scrolls and/or weapons
       int itemCount = randInt(2,3);
       for(int i = 0; i< itemCount; i++){
           int r;
           int c;
           do{
               r = randInt(3,15);
               c = randInt(6,60);
           }while(isWall(r, c));
           if(trueWithProbability(0.5)){
               m_grid[r][c] = ')';                  ///see Dungeon.h for guide of objects
               m_objects[r][c] = randInt(1,3);      ///choose random droppable weapon
           } else{
               m_grid[r][c] = '?';
               m_objects[r][c] = randInt(7, 10);    ///choose random droppable scroll
           }
       }
       ///adds staircase/golden idol
       int r;
       int c;
       do{
           r = randInt(3,15);
           c = randInt(6,60);
       }while(m_grid[r][c] != ' ');
       if(m_level !=4)
           m_grid[r][c] = '>';
       else ///adds golden item if level 4
           m_grid[r][c] = '&';
}

//Private Functions for generateDungeon/////////////////////////////////////////////////////////////////
bool Dungeon::overlap(room curr) const{ ///checks to see if there is a room in the space for a new room
    for(int r = curr.row; r < curr.row + curr.width; r++){
        for(int c = curr.col; c < curr.col + curr.length; c++){
            if(m_grid[r][c] == ' ' || m_grid[r-1][c] == ' ' || m_grid[r+1][c] == ' ' || m_grid[r][c-1] == ' ' || m_grid[r][c+1] == ' ')
                return true;
        }
    }
    return false;
}

void Dungeon::connectRooms(int curCR, int curCC, int lastCR, int lastCC){   ///set current spot as empty, and recursively get closer to next room center
    m_grid[curCR][curCC] = ' ';
    if(curCR == lastCR && curCC == lastCC)
        return;
    if(curCR < lastCR)
        return connectRooms(curCR+1, curCC, lastCR, lastCC);
    else if (curCR > lastCR)
        return connectRooms(curCR-1, curCC, lastCR, lastCC);
    else if (curCC < lastCC)
        return connectRooms(curCR, curCC+1, lastCR, lastCC);
    else
        return connectRooms(curCR, curCC-1, lastCR, lastCC);
}
