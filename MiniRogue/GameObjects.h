
#ifndef GameObjects_h
#define GameObjects_h

#include <string>

//Base Class of all objects//////////////////////////////////////////////
class GameObject{   ///Abstract Class
public:
    //Constructor/Destructor
    GameObject(std::string name);
    virtual ~GameObject();
    //Accessor Functions
    std::string name() const;
    virtual std::string action() const = 0;
private:
    std::string m_name;
};

//Weapon Object///////////////////////
class Weapon : public GameObject{
public:
    //Constructor/Destructor
    Weapon(std::string name);
    virtual ~Weapon();
    //Accessor Functions
    int dexterity() const;
    int damage() const;
    virtual std::string action() const;
private:
    int m_dexterity;
    int m_damage;
};


//Scroll Object///////////////////////
class Scroll : public GameObject{
public:
    //Constructor/Destructor
    Scroll(std::string name);
    virtual ~Scroll();
    //Accessor Functions
    virtual std::string action() const;
};

#endif /* GameObjects_h */
