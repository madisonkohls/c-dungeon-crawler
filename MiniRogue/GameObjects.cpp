
#include "GameObjects.h"

using namespace std;

//Game Object Implementation//////////////////////////////////////////////
//Constructor/Destructor/////////
GameObject::GameObject(string name): m_name(name){}

GameObject::~GameObject(){}

//Accessor Functions/////////
string GameObject::name() const{
    return m_name;
}

//Weapon Implementation///////////////////////////
//Constructor/Destructor/////////
Weapon::Weapon(string name) : GameObject(name){
    if(name == "Mace"){
        m_damage = 2;
        m_dexterity = 0;
    } else if (name == "Short Sword"){
        m_damage = 2;
        m_dexterity = 0;
    } else if (name == "Long Sword"){
        m_damage = 4;
        m_dexterity = 2;
    } else if (name == "Magic Axe"){
        m_damage = 5;
        m_dexterity = 5;
    } else {
        m_damage = 2;
        m_dexterity = 3;
    }
}

Weapon::~Weapon(){}

//Accessor Functions/////////
int Weapon::dexterity() const{
    return m_dexterity;
}

int Weapon::damage() const{
    return m_damage;
}

string Weapon::action() const{
    string s = name();
    if(s=="Mace")
        return "swings mace";
    else if(s=="Short Sword")
        return "swings short sword";
    else if(s=="Long Sword")
        return "swings long sword";
    else if(s=="Magic Axe")
        return "swings magic axe";
    else
        return "chucks magic fangs";
}

//Scroll Implementation///////////////////////////
//Constructor/Destructor/////////
Scroll::Scroll(string name) : GameObject(name){}

Scroll::~Scroll(){}

//Accessor Functions/////////
string Scroll::action() const{
    string s = name();
    if(s=="Scroll of Teleportation")
        return "You saw light flash before your eyes. Pretty sure you saw the answer to life as well. But you're still in this dungeon.";
    else if(s=="Scroll of Improve Armor")
        return "A glisten of iron wraps your body. You feel bulky, but protected.";
    else if(s=="Scroll of Raise Strength")
        return "Your muscles pulsate. Finally something easier than protein shakes!";
    else if(s=="Scroll of Enhance Health")
        return "You don't sense anything, but you feel better.";
    else
        return "A fly buzzes past and you squish it between you thumb and index finger, nice.";
}
