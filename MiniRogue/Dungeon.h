
#ifndef Dungeon_h
#define Dungeon_h

#include <string>
#include <vector>

const int MAXROWS = 18;
const int MAXCOLS = 70;

class Player;
class Monster;

class Dungeon{
public:
    //Constructor/Destructor
    Dungeon(int level, int gobSmell);
    ~Dungeon();
    //Acccessor Functions
    int level() const;
    Player* player() const;
    void display(std::string msg) const;
    bool isWall(const int& r, const int&  c) const;
    char location(const int& r, const int& c) const;
    int objectType(const int& r, const int& c) const;
    char actorType(const int& r, const int& c) const;
    int monsterIndex(const int& r, const int& c) const;
    Monster* monster(int index) const;
    int monsterSize() const;
    int goblinSmell() const;
    //Mutator Functions
    void generateDungeon();
    void addMonsters();
    void deleteMonster(const int& i);
    void deleteAllMonsters();
    void addPlayer();
    void increaseLevel();
    void movePlayer();
    void setLocation(const int& r, const int& c, int type);
    void setActor(const int& r, const int& c, const char& ch);
private:
    char m_grid[MAXROWS][MAXCOLS];
    char m_actors[MAXROWS][MAXCOLS];
    /*
     ACTOR GRIDY KEY:
     ' ': no actor
     '@': player
     'g': goblin
     's': snakewoman
     'b': bogeyman
     'd': dragon
     */
    int m_objects[MAXROWS][MAXCOLS];
    /*
      OBJECT GRID KEY:
     0: No Object
     1: Short Sword
     2: Long Sword
     3: Mace
     4: Magic Axe
     5: Magic Fangs of Sleep
     6: Scroll of Teleportation
     7: Scroll of Improve Armor
     8: Scroll of Raise Strength
     9: Scroll of Enhance Health
     10: Scroll of Enhance Dexterity
      */
    int m_level;
    Player* m_player;
    std::vector<Monster*> m_monsters;
    int m_goblinSmell;
    //Priavate Structures
    struct room{
        int col;
        int row;
        int length;
        int width;
    };
    //Private Functions
    bool overlap(struct room) const;
    void connectRooms(int curCR, int curCC, int lastCR, int lastCC);
};

#endif /* Dungeon_h */
