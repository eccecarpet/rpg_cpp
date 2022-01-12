#include <iostream>
#include "Player.h"

using namespace std;

// Pointer of the only player available in the game to be accessed through getPlayer static function 
static Player* instance;

Location* Player::location = nullptr;
int Player::steps = -1;
vector<Item*> Player::inventory;

//public functions
bool Player::hasItem(Item* i) const
{
    bool result = false;
    if (i)
    {
        for (auto& it : inventory)
        {
            if (*it == *i)
            {
                result = true;
            }
        }
    }
    return result;
}

bool Player::takeItem(Item* i) // if it is a valid ptr
{
    bool result = false;
    if (i)
    {
        inventory.push_back(i);// add to the inventory
        result = true;
    }
    return result;
}

bool Player::dropItem(Item* item)
{
    int result = false;
    int index = -1;
    //only try to delete if the pointer is valid
    if (item)
    {
        //finds the element to delete
        for (unsigned int i = 0; i < inventory.size(); i++)
        {
            if (inventory[i] == item)
            {
                index = i;
                break;
            }
        }

        // If the item is in the current inventory
        if (index >= 0)
        {
            inventory.erase(inventory.begin() + index);
            result = true;
        }
    }
    return result;
}

bool Player::openItem(Item* item)
{
    bool result = false;

    //  try to open the item if it is a valid ptr only
    if (item)
    {
        Item* neededItem = item->getKeyItem();
        Item* myItem = nullptr;
        // if the item needs something to open it, search in the inventory
        if (neededItem)
        {
            for (unsigned int index = 0; index < inventory.size(); index++)
            {
                if (inventory[index] == neededItem)
                {
                    myItem = inventory[index];
                    break;
                }
            }
        }
        // open the item
        result = item->open(myItem);
        if (result)
        {
            
            vector<Item*>& contents = item->getContents();
            int contents_size = contents.size();
            for (unsigned int i = 0; i < contents_size; i++)
            {
                //moves all the elements of the Container into our inventory and eliminates them from the Container
                inventory.push_back(contents.back());
                contents.pop_back();
            }
        }
    }
    return result;
}

void Player::printStatus() const
{
    cout << "Current Location:" << location->getName() << endl;
    cout << "Your inventory contains :" << endl;
    for (unsigned int i = 0; i < inventory.size(); i++)
    {
        cout << inventory[i]->getName() << ", " << inventory[i]->getDescription() << endl;
    }
    cout << "Steps Counter:" << steps << endl;
}

void Player::setLocation(Location* loc)
{
    steps++;
    location = loc;
}

//static functions
Player Player::getPlayer()
{
    if (!instance)
        instance = new Player();
    return *instance;
}

//private functions
Player::Player()
{
    Player::location = nullptr;
    inventory.reserve(10);
}

Location* Player::getLocation() const
{
    return location;
}

vector<Item*> Player::getInventory() const
{
    return inventory;
}
