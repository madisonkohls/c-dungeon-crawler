
#ifndef Actors_h
#define Actors_h

#include <string>
#include <vector>

class Dungeon;
class Weapon;
class GameObject;

//Base Class of all Actors//////////////////////////////////////////////////////////////////////////////////////
class Actor{ ///Abstract
public:
    //Constructor/Destructor
    Actor(Dungeon* dp, int r, int c, int hp, Weapon* weapon, int armor, int strength, int dexterity);
    virtual ~Actor();
    //Accessor Functions
    Dungeon* dungeon() const;
    Weapon* weapon() const;
    int row() const;
    int col() const;
    int hp() const;
    int armor() const;
    int strength() const;
    int dexterity() const;
    int sleep() const;
    bool isDead() const;
    //Mutator Functions
    std::string attemptMove(const char& dir, const char& ch);
    void setRow(const int& r);
    void setCol(const int& c);
    void addSleep(const int& s);
    void changeWeapon(Weapon* wp);
    void armorIncrease(const int& a);
    void strengthIncrease(const int& s);
    void HPIncrease(const int& h);
    void dexterityIncrease(const int& d);
    virtual std::string attack(int index = -1) = 0;
private:
    Dungeon* m_dungeon;
    Weapon* m_weapon;
    int m_row;
    int m_col;
    int m_hp;
    int m_armor;
    int m_strength;
    int m_dexterity;
    int m_sleep;
};

//Player Actor///////////////////////////////////////
class Player : public Actor{
public:
    //Constructor/Destructor
    Player(Dungeon* dp, int r, int c);
    virtual ~Player();
    //Accessor Functions
    bool wonGame() const;
    void viewInventory() const;
    int decodeInventory(const char& c) const;
    int maxHP() const;
    //Mutator Functions
    std::string move(char ch);
    void teleport(const int& r, const int& c);
    std::string pickUpObject();
    std::string equipWeapon();
    std::string readScroll();
    void setCheat();
    void applyScroll(std::string name);
    void maxHPIncrease(const int& h);
    void removeItem(const int& index);
    virtual std::string attack(int index);
private:
    std::vector<GameObject*> m_inventory;
    bool gameStatus;
    int m_maxHP;
};

//Monster Actor///////////////////////////////////////////////////////////////
class Monster : public Actor{   ///Abstract
public:
    //Constructor/Destructor
    Monster(Dungeon* dp, int r, int c, int hp, Weapon* weapon, int armor, int strength, int dexterity, int smell, char ch);
    virtual ~Monster();
    //Accessor Functions
    char typeMonster() const;
    bool smellPlayerThroughWall() const;
    int getSmellDistance() const;
    //Mutator Functions
    virtual std::string move();
    virtual std::string attack(int index = -1);
    virtual void dropLoot() = 0;
private:
    int m_smellDistance;
    char m_type;
};

//Bogeyman Monster/////////////////////////////////////
class Bogeyman : public Monster{
public:
    //Constructor/Destructor
    Bogeyman(Dungeon* dp, int r, int c);
    virtual ~Bogeyman();
    //Mutator Functions
    virtual void dropLoot();
    virtual std::string move();
};

//Snakewoman Monster////////////////////////////////////
class Snakewoman : public Monster{
public:
    //Constructor/Destructor
    Snakewoman(Dungeon* dp, int r, int c);
    virtual ~Snakewoman();
    //Mutator Functions
    virtual void dropLoot();
    virtual std::string move();
};

//Dragon Monster////////////////////////////////////////
class Dragon : public Monster{
public:
    //Constructor/Destructor
    Dragon(Dungeon* dp, int r, int c);
    virtual ~Dragon();
    //Mutator Functions
    virtual void dropLoot();
    virtual std::string move();
};

//Goblin Monster////////////////////////////////////////
class Goblin : public Monster{
public:
    //Constructor/Destructor
    Goblin(Dungeon* dp, int r, int c, int smellDist);
    virtual ~Goblin();
    //Mutator Functions
    virtual void dropLoot();
    virtual std::string move();
private:
    int min(const int& a, const int& b) const;
    int minimum(int a, int b, int c, int d) const;
    void shortestPath(char maze[18][70], int sr, int sc, int& mindist, int dist, int count);
};

#endif /* Actors_h */
