
#include "Actors.h"
#include "Dungeon.h"
#include "utilities.h"
#include "GameObjects.h"
#include <iostream>
#include <cmath>

using namespace std;

//Actor Implementation///////////////////////////////////////////////////////////////////////////////////////////////
//Constructor/Destructor/////////
Actor::Actor(Dungeon* dp, int r, int c, int hp, Weapon* weapon, int armor, int strength, int dexterity) : m_row(r), m_col(c), m_hp(hp), m_armor(armor), m_strength(strength), m_dexterity(dexterity), m_sleep(0){
    m_dungeon = dp;
    m_weapon = weapon;
}

Actor::~Actor(){
    delete m_weapon;
}

//Accessor Functions/////////
Dungeon* Actor::dungeon() const{
    return m_dungeon;
}

Weapon* Actor::weapon() const{
    return m_weapon;
}

int Actor::row() const{
    return m_row;
}

int Actor::col() const{
    return m_col;
}

int Actor::hp() const{
    return m_hp;
}

int Actor::armor() const{
    return m_armor;
}
int Actor::strength() const{
    return m_strength;
}

int Actor::dexterity() const{
    return m_dexterity;
}

int Actor::sleep() const{
    return m_sleep;
}

bool Actor::isDead() const{
    if(m_hp > 0)
        return false;
    return true;
}

//Mutator Functions/////////
void Actor::setRow(const int& r){
    m_row = r;
}

void Actor::setCol(const int& c){
    m_col = c;
}

void Actor::addSleep(const int& s){
    if(m_sleep+s < 10)
        m_sleep += s;
}

void Actor::changeWeapon(Weapon* wp){
    m_weapon = wp;
}

void Actor::armorIncrease(const int& a){
    if(m_armor+a <100)
        m_armor += a;
}

void Actor::strengthIncrease(const int& s){
    if(m_strength+s <100)
        m_strength += s;
}

void Actor::dexterityIncrease(const int& d){
    if(m_dexterity+d <100)
        m_dexterity += d;
}

void Actor::HPIncrease(const int& h){
    m_hp += h;
}

string Actor::attemptMove(const char& dir, const char& ch){
    switch (dir){
      case ARROW_UP:
            if (m_dungeon->isWall(m_row-1, m_col) || (ch != '@' && m_dungeon->actorType(m_row-1, m_col) != '@' && m_dungeon->actorType(m_row-1, m_col) != ' '))
                return "blocked";       ///If direction is blocked by a wall or you are a monster and there is another monster in the way, return "blocked"
            else if(ch == '@' && m_dungeon->actorType(m_row-1, m_col) != ' ')
                return attack(m_dungeon->monsterIndex(m_row-1, m_col));         ///If you are the player and you run into a monster, attack
            else if(ch != '@' && m_dungeon->actorType(m_row-1, m_col) == '@')
                return attack();        ///If you are a monster and run into the player, attack
            else{
                m_dungeon->setActor(m_row, m_col, ' ');     ///reset the actor grid appropriately and your own coordinates
                m_row--;
                m_dungeon->setActor(m_row, m_col, ch);
            }
            break;
      case ARROW_LEFT:
            if (m_dungeon->isWall(m_row,m_col-1) || (ch != '@' && m_dungeon->actorType(m_row, m_col-1) != '@' && m_dungeon->actorType(m_row, m_col-1) != ' '))
                return "blocked";
            else if(ch == '@' && m_dungeon->actorType(m_row, m_col-1) != ' ')
                return attack(m_dungeon->monsterIndex(m_row, m_col-1));
            else if(ch != '@' && m_dungeon->actorType(m_row, m_col-1) == '@')
                return attack();
            else{
                m_dungeon->setActor(m_row, m_col, ' ');
                m_col--;
                m_dungeon->setActor(m_row, m_col, ch);
            }
            break;
      case ARROW_DOWN:
            if (m_dungeon->isWall(m_row+1,m_col) || (ch != '@' && m_dungeon->actorType(m_row+1, m_col) != '@' && m_dungeon->actorType(m_row+1, m_col) != ' '))
                return "blocked";
            else if(ch == '@' && m_dungeon->actorType(m_row+1, m_col) != ' ')
                return attack(m_dungeon->monsterIndex(m_row+1, m_col));
            else if(ch != '@' && m_dungeon->actorType(m_row+1, m_col) == '@')
                return attack();
            else{
                m_dungeon->setActor(m_row, m_col, ' ');
                m_row++;
                m_dungeon->setActor(m_row, m_col, ch);
            }
            break;
      case ARROW_RIGHT:
            if (m_dungeon->isWall(m_row, m_col+1) || (ch != '@' && m_dungeon->actorType(m_row, m_col+1) != '@' && m_dungeon->actorType(m_row, m_col+1) != ' '))
                return "blocked";
            else if(ch == '@' && m_dungeon->actorType(m_row, m_col+1) != ' ')
                return attack(m_dungeon->monsterIndex(m_row, m_col+1));
            else if(ch != '@' && m_dungeon->actorType(m_row, m_col+1) == '@')
                return attack();
            else{
                m_dungeon->setActor(m_row, m_col, ' ');
                m_col++;
                m_dungeon->setActor(m_row, m_col, ch);
            }
            break;
      default:
            return "";
    }
    return "";
}

//Player Implementation///////////////////////////////////////////////////////
//Constructor/Destructor/////////
Player::Player(Dungeon* dp, int r, int c): Actor(dp, r, c, 20, new Weapon("Short Sword"), 2, 2, 2), gameStatus(false), m_maxHP(20){
    m_inventory.push_back(weapon());
}

Player::~Player(){
    while(m_inventory.size() != 0){
        Weapon* wp = dynamic_cast<Weapon*>(m_inventory[m_inventory.size()-1]);
        if(wp != weapon())
            delete m_inventory[m_inventory.size()-1];   ///delete all other newly allocated objects in inventory
        m_inventory.pop_back();                         ///weapon will be deleted by the actor base class
    }
}

//Accessor Functions/////////
bool Player::wonGame() const{
    return gameStatus;
}

int Player::maxHP() const{
    return m_maxHP;
}

void Player::viewInventory() const{
    cout << "Inventory: " << endl;
    char index = 'a';
    for(int i = 0; i < m_inventory.size(); i++){
        cout << index << ". " << m_inventory[i]->name() << endl;
        index++;
    }
}

int Player::decodeInventory(const char& c) const{
    int index = 0;
    if(c < 'a' || c > 'z')
        return -1;
    char start = 'a';
    while(start != c){
        start++;
        index++;
    }
    return index;
}

//Mutator Functions/////////
void Player::setCheat(){
    int s = strength();
    strengthIncrease(9-s);
    m_maxHP = 50;
    int h = hp();
    HPIncrease(50-h);
}

string Player::attack(int index){
    string mons;    ///for output message
    switch(dungeon()->monster(index)->typeMonster()){
        case 'g':
            mons = "Goblin";
            break;
        case 's':
            mons = "Snakewoman";
            break;
        case 'b':
            mons = "Bogeyman";
            break;
        case 'd':
            mons = "Dragon";
            break;
    }
    string msg = "Player " + weapon()->action() + " at " + mons + " and ";
    int attackerPoints = dexterity() + weapon()->dexterity();
    int defenderPoints = dungeon()->monster(index)->dexterity() + dungeon()->monster(index)->armor();
    if(randInt(1, attackerPoints) >= randInt(1,defenderPoints)){    ///if player hits the monster
        int damagePoints = randInt(0, strength()+weapon()->damage()-1);
        dungeon()->monster(index)->HPIncrease(-damagePoints);
        if(dungeon()->monster(index)->isDead()){                    ///if monster is dead delete it
            if(dungeon()->objectType(dungeon()->monster(index)->row(), dungeon()->monster(index)->col())==0 && dungeon()->location(dungeon()->monster(index)->row(), dungeon()->monster(index)->col()) != '>' && dungeon()->location(dungeon()->monster(index)->row(), dungeon()->monster(index)->col()) != '&' )
                dungeon()->monster(index)->dropLoot();
            dungeon()->deleteMonster(index);
            msg += "deals final blow.";
        }else if(weapon()->name() == "Magic Fangs of Sleep" && trueWithProbability(0.2)){   ///if weapon put monster to sleep
            int newSleep = randInt(2,6);
            if(dungeon()->monster(index)->sleep() == 0)             ///if monster is asleep already or not
                dungeon()->monster(index)->addSleep(newSleep);
            else if (dungeon()->monster(index)->sleep() < newSleep){
                dungeon()->monster(index)->addSleep(-1*(dungeon()->monster(index)->sleep()));
                dungeon()->monster(index)->addSleep(newSleep);
            }
            msg += "puts monster to sleep.";
        }else{                  ///monster is hit but is not dead or asleep
            msg += "hits.";
        }
    } else {    ///monster is not hit
        msg += "misses.";
    }
    return msg;
}

string Player::pickUpObject(){
    string msg = "You pick up a ";
    switch(dungeon()->objectType(row(), col())){
        case 1:
            m_inventory.push_back(new Weapon("Short Sword"));
            break;
        case 2:
            m_inventory.push_back(new Weapon("Long Sword"));
            break;
        case 3:
            m_inventory.push_back(new Weapon("Mace"));
            break;
        case 4:
            m_inventory.push_back(new Weapon("Magic Axe"));
            break;
        case 5:
            m_inventory.push_back(new Weapon("Magic Fangs of Sleep"));
            break;
        case 6:
            m_inventory.push_back(new Scroll("Scroll of Teleportation"));
            break;
        case 7:
            m_inventory.push_back(new Scroll("Scroll of Improve Armor"));
            break;
        case 8:
            m_inventory.push_back(new Scroll("Scroll of Raise Strength"));
            break;
        case 9:
            m_inventory.push_back(new Scroll("Scroll of Enhance Health"));
            break;
        case 10:
            m_inventory.push_back(new Scroll("Scroll of Enhance Dexterity"));
            break;
    }
    dungeon()->setLocation(row(), col(), 0);
    return msg + m_inventory.back()->name() + ".";
}

void Player::removeItem(const int& index){
    vector<GameObject*>::iterator it = m_inventory.begin();
    for(int i = 0; i < index; i++)
        it++;
    delete *it;
    m_inventory.erase(it);
}

string Player::equipWeapon(){
    int index = decodeInventory(getCharacter());
    if(index >= m_inventory.size() || index < 0)
        return "You can't equip something not in your inventory.";
    Weapon* cw = dynamic_cast<Weapon*>(m_inventory[index]);
    if(cw != nullptr){
        changeWeapon(cw);
        return "You have equipped a " + m_inventory[index]->name() + ".";
    }else{
        return "You try swinging the flimsy " + m_inventory[index]->name() + " but quickly realize \nits ineffiency as a weapon.";
    }
}

string Player::readScroll(){
    int index = decodeInventory(getCharacter());
    if(index >= m_inventory.size() || index < 0)
        return "You can't equip something not in your inventory.";
    Scroll* cw = dynamic_cast<Scroll*>(m_inventory[index]);
    if(cw != nullptr){
        applyScroll(m_inventory[index]->name());
        string name = m_inventory[index]->name();
        string action = m_inventory[index]->action();
        removeItem(index);
        return "You read the " + name + ".\n" + action;
    }else{
        return "You try reading the " + m_inventory[index]->name() + ", but alas you're just crazy.";
    }
}

void Player::applyScroll(string name){
    string s = name;
    if(s=="Scroll of Teleportation"){
        int rPlayer;
        int cPlayer;
        do{
            rPlayer = randInt(3,15);
            cPlayer = randInt(6,60);
        }while(dungeon()->isWall(rPlayer, cPlayer) || dungeon()->actorType(rPlayer, cPlayer) != ' ');
        teleport(rPlayer, cPlayer);
    }else if(s=="Scroll of Improve Armor")
        armorIncrease(randInt(1,3));
    else if(s=="Scroll of Raise Strength")
        strengthIncrease(randInt(1,3));
    else if(s=="Scroll of Enhance Health")
        maxHPIncrease(randInt(3,8));
    else
        dexterityIncrease(1);
}

string Player::move(char ch){
    ///possible increase hp before each turn
    if(trueWithProbability(0.1) && hp() < maxHP())
        HPIncrease(1);
    if(sleep()>0){
        addSleep(-1);
        return "";
    }
    switch(tolower(ch)){
        case 'g':   ///PICKING UP AN OBJECT
            if(dungeon()->location(row(), col()) == '&'){
                gameStatus = true;
                return "";
            } else if(dungeon()->objectType(row(), col()) > 0){
                if(m_inventory.size() > 25)
                    return "Your inventory is already full.";
                return pickUpObject();
            } else{
                return "";
            }
            break;
        case 'i':   ///VIEWING INVENTORY
            clearScreen();
            viewInventory();
            while(!getCharacter());///won't display screen until player hits a character
            return "";
            break;
        case 'r':   ///SELECTING A SCROLL
            clearScreen();
            viewInventory();
            return readScroll();
            break;
        case 'w':   ///SELECTING A WEAPON
            clearScreen();
            viewInventory();
            return equipWeapon();
            break;
        case '>':   ///GOING DOWNSTAIRS
            if(dungeon()->location(row(), col()) == '>')
                return "CHANGELEVEL";
            return "";
            break;
        case 'c':   ///CHEATS
            setCheat();
            return "";
            break;
        default:    ///ATTEMPTING TO MOVE/DO NOTHING
            return attemptMove(ch, '@');
            break;
    }
}

void Player::teleport(const int& r, const int& c){
    dungeon()->setActor(row(), col(), ' ');
    setRow(r);
    setCol(c);
    dungeon()->setActor(row(), col(), '@');
}

void Player::maxHPIncrease(const int& h){
    m_maxHP += h;
}

//Monster Implementation///////////////////////////////////////////////////////////////////////////////////////////////
//Constructor/Destructor/////////
Monster::Monster(Dungeon* dp, int r, int c, int hp, Weapon* weapon, int armor, int strength, int dexterity, int smell, char ch) : Actor(dp, r, c, hp, weapon, armor, strength, dexterity), m_smellDistance(smell), m_type(ch){}

Monster::~Monster(){}

//Accessor Functions/////////
int Monster::getSmellDistance() const{
    return m_smellDistance;
}

char Monster::typeMonster() const{
    return m_type;
}

bool Monster::smellPlayerThroughWall() const{
    int difference = abs(row()-dungeon()->player()->row()) + abs(col()-dungeon()->player()->col());
    if(difference <= m_smellDistance)
        return true;
    else
        return false;
}

//Mutator Functions/////////
string Monster::attack(int index){
    if(index!=-1)
        return "";  ///index is not necessary since monsters always attack the player, in any case index is something else return ""
    string msg;
    int attackerPoints = dexterity() + weapon()->dexterity();
    int defenderPoints = dungeon()->player()->dexterity() + dungeon()->player()->armor();
    if(randInt(1, attackerPoints) >= randInt(1,defenderPoints)){
        int damagePoints = randInt(0, strength()+weapon()->damage()-1);
        dungeon()->player()->HPIncrease(-damagePoints);
        if(dungeon()->player()->isDead()){
            msg = "deals final blow.";
        }else if(weapon()->name() == "Magic Fangs of Sleep" && trueWithProbability(0.2)){
            int newSleep = randInt(2,6);
            if(dungeon()->player()->sleep() == 0)
                dungeon()->player()->addSleep(newSleep);
            else if (dungeon()->player()->sleep() < newSleep){
                dungeon()->player()->addSleep(-1*(dungeon()->player()->sleep()));
                dungeon()->player()->addSleep(newSleep);
            }
            msg = "puts player to sleep.";
        }else{
            msg = "hits.";
        }
    } else {
        msg = "misses.";
    }
    return msg;
}

string Monster::move(){
    if(sleep()>0){
        addSleep(-1);
        return "";
    } else if (smellPlayerThroughWall()){
        if(dungeon()->player()->row() > row()){
            string msg = attemptMove('j', m_type);
            if(msg != "blocked")                    ///if player is blocked, try another move
                return msg;
        }
        if(dungeon()->player()->row() < row()){
            string msg = attemptMove('k', m_type);
            if(msg != "blocked")
                return msg;
        }
        if(dungeon()->player()->col() > col()){
            string msg = attemptMove('l', m_type);
            if(msg != "blocked")
                return msg;
        }
        if(dungeon()->player()->col() < col()){
            string msg = attemptMove('h', m_type);
            if(msg != "blocked")
                return msg;
        }
    }
    return "";
}


//Bogeyman Implementation///////////////////////////////////////////////////////
//Constructor/Destructor/////////
Bogeyman::Bogeyman(Dungeon* dp, int r, int c) : Monster(dp, r, c, randInt(5,10), new Weapon("Short Sword"), 2, randInt(2,3), randInt(2,3), 5, 'b'){}

Bogeyman::~Bogeyman(){}

//Mutator Functions/////////
void Bogeyman::dropLoot(){
    if(trueWithProbability(0.1)){
        dungeon()->setLocation(row(), col(), 4);
    }
}

string Bogeyman::move(){
    string msg = Monster::move();
    if(msg != "")
        return "\nBogeyman "+ weapon()->action() + " at Player and " + msg;
    return msg;
}

//Snakewoman Implementation///////////////////////////////////////////////////////
//Constructor/Destructor/////////
Snakewoman::Snakewoman(Dungeon* dp, int r, int c) : Monster(dp, r, c, randInt(3,6), new Weapon("Magic Fang"), 3, 2, 3, 3, 's'){}

Snakewoman::~Snakewoman(){}

//Mutator Functions/////////
void Snakewoman::dropLoot(){
    if(trueWithProbability(0.33)){
        dungeon()->setLocation(row(), col(), 5);
    }
}

string Snakewoman::move(){
    string msg = Monster::move();
    if(msg != "")
        return "\nSnakewoman "+ weapon()->action() + " at Player and " + msg;
    return msg;
}

//Dragon Implementation///////////////////////////////////////////////////////
//Constructor/Destructor/////////
Dragon::Dragon(Dungeon* dp, int r, int c) : Monster(dp, r, c, randInt(20,25), new Weapon("Long Sword"), 4, 4, 4, 1, 'd'){}

Dragon::~Dragon(){}

//Mutator Functions/////////
void Dragon::dropLoot(){
    dungeon()->setLocation(row(), col(), randInt(6,10));
}

string Dragon::move(){
    ///possible increase hp before each turn
    if(trueWithProbability(0.1) && hp() < 99)
        HPIncrease(1);
    string msg = Monster::move();
    if(msg != "")
        return "\nDragon "+ weapon()->action() + " at Player and " + msg;
    return msg;
}

//Goblin Implementation///////////////////////////////////////////////////////
//Constructor/Destructor/////////
Goblin::Goblin(Dungeon* dp, int r, int c, int smellDist) : Monster(dp, r, c, randInt(15,20), new Weapon("Short Sword"), 1, 3, 1, smellDist, 'g'){}

Goblin::~Goblin(){}

//Mutator Functions/////////
void Goblin::dropLoot(){
    if(trueWithProbability(0.33)){
        dungeon()->setLocation(row(), col(), randInt(4,5));
    }
}

string Goblin::move(){
    if(sleep()>0){
        addSleep(-1);
        return "";
    }
    int minimumdist = getSmellDistance();   ///make the minimum distance smell range
    char maze[MAXROWS][MAXCOLS];
    for(int i = 0; i < MAXROWS; i++){   ///create a grid to check path
        for(int j = 0; j < MAXCOLS; j++){
            maze[i][j] = dungeon()->location(i,j);
            if(dungeon()->actorType(i, j) == 'g' || dungeon()->actorType(i, j) == 's' || dungeon()->actorType(i, j) == 'd' || dungeon()->actorType(i, j) == 'b')
                maze[i][j] = '#';
        }
    }
    shortestPath(maze, row(), col()-1, minimumdist, 0, getSmellDistance()-1);   ///calculate shortest path going left
    int h = minimumdist + 1;
    minimumdist = getSmellDistance();
    shortestPath(maze, row()+1, col(), minimumdist, 0, getSmellDistance()-1);   ///calculate shortest path going down
    int j =  minimumdist + 1;
    minimumdist = getSmellDistance();
    shortestPath(maze, row()-1, col(), minimumdist, 0, getSmellDistance()-1);   ///calculate shortest path going up
    int k =  minimumdist + 1;
    minimumdist = getSmellDistance();
    shortestPath(maze, row(), col()+1, minimumdist, 0, getSmellDistance()-1);   ///calculate shortest path going right
    int l =  minimumdist + 1;
    minimumdist = getSmellDistance();
    int move = minimum(h,j,k,l);    ///make the shortest out of all the options the move
    if(move <= getSmellDistance()){
        if(move == h){  ///get the minimum move and return the string it returns
            string msg = attemptMove(ARROW_LEFT, 'g');
            if(msg != "")
                return "\nGoblin "+ weapon()->action() + " at Player and " + msg;
            return msg;
        }
        if(move == j){
            string msg = attemptMove(ARROW_DOWN, 'g');
            if(msg != "")
                return "\nGoblin "+ weapon()->action() + " at Player and " + msg;
            return msg;
        }
        if(move == k){
            string msg = attemptMove(ARROW_UP, 'g');
            if(msg != "")
                return "\nGoblin "+ weapon()->action() + " at Player and " + msg;
            return msg;
        }
        if(move == l){
            string msg = attemptMove(ARROW_RIGHT, 'g');
            if(msg != "")
                return "\nGoblin "+ weapon()->action() + " at Player and " + msg;
            return msg;
        }
    }
    return "";
}

//Private Helper Functions/////////
void Goblin::shortestPath(char maze[18][70], int sr, int sc, int& mindist, int dist, int count){
    if(maze[sr][sc] == '#'){
        return;
    }
    if(sr == dungeon()->player()->row() && sc == dungeon()->player()->col()){
        mindist = min(mindist, dist);
        return;
    }
    if(count <= 0){
        maze[sr][sc] = ' ';
        return;
    }
    maze[sr][sc] = '#';
    if(maze[sr+1][sc] != '#'){  ///Checking down
        shortestPath(maze, sr+1, sc, mindist, dist+1, count-1);
    }
    if(maze[sr-1][sc] != '#'){  ///Checking up
        shortestPath(maze, sr-1, sc, mindist, dist+1, count-1);
    }
    if(maze[sr][sc+1] != '#'){  ///Checking right
        shortestPath(maze, sr, sc+1, mindist, dist+1, count-1);
    }
    if(maze[sr][sc-1] != '#'){  ///Checking left
        shortestPath(maze, sr, sc-1, mindist, dist+1, count-1);
    }
    maze[sr][sc] = ' '; ///backtrack from position if can't move
}

int Goblin::min(const int& a, const int& b) const{
    if(a<b)
        return a;
    return b;
}

int Goblin::minimum(int a, int b, int c, int d) const{
    if(a <= b)
        if (a <= c)
            if(a <= d)
                return a;
    if(b < a)
        if (b <= c)
            if (b <= d)
                return b;
    if(c < a)
        if (c < b)
            if (c <= d)
                return c;
    return d;
}
