#pragma once
#include "Location.h"
#include "Item.h"

class Player
{
public:
    bool hasItem(Item* i) const;
    bool takeItem(Item* i);
    bool dropItem(Item* item);
    bool openItem(Item* item);
    Location* getLocation() const;
    vector<Item*> getInventory() const;
    void printStatus() const;
    void setLocation(Location* location);
    static Player getPlayer();
private:
    Player();
    static Location* location;
    static int steps;
    static vector<Item*> inventory;
};
