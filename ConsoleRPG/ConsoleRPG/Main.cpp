#include <iostream>
#include <vector>
#include "Location.h"
#include "Item.h"
#include "Player.h"
#include "GameFeatures.h"

using namespace std;


//class declarations
void playGame(Player& player);
void lookMenu(Player& player);
void takeMenu(Player& player);
void dropMenu(Player& player);
void openMenu(Player& player);
void moveMenu(Player& player);


int main()
{
    Player player = Player::getPlayer();
    playGame(player);
    return 0;
}

//supplementary functions
void playGame(Player& player)
{
    bool finished = false;
    vector<Location*> locations;
    vector<Item*> items;
    readData(items, locations, "GamesInput.ini"); // read from  txt or ini file
    // sets initial location of the player
    player.setLocation(locations.at(0));

    while (!finished)
    {
       cout << "**************************" << endl;
       cout <<endl;
        player.printStatus();
        //switch statement handling the different commands 
        switch (getUserAction())
        {
        case Command::LOOK:
            lookMenu(player);
            break;
        case Command::TAKE:
            takeMenu(player);
            break;
        case Command::DROP:
            dropMenu(player);
            break;
        case Command::OPEN:
            openMenu(player);
            break;
        case Command::MOVE:
            moveMenu(player);
            break;
        case Command::QUIT:
            finished = true;
        default:
            break;
        }
        // last location in the locations vector is the goal location
        if (player.getLocation() == nullptr)
        {
            finished = true;
            std::cout << "Congratulations! you have escaped the room safely" << std::endl;
        }
    }
}

void lookMenu(Player& player)
{
    Location* location = player.getLocation();
    if (location)
    {
        location->print();
    }
}

void takeMenu(Player& player)
{
    Location* location = player.getLocation();
    vector<Item*> contents;
    if (location)
    {
        contents = location->getContents();
        if (contents.size() > 0)
        {
            bool finished = false;
            string input;
            Item* item = nullptr;
            cout << "Choose from the following items" << endl;
            for (unsigned int i = 0; i < contents.size(); i++)
            {
                cout << " - " << contents[i]->getName() << endl;
            }
            while (false == finished)
            {
                cout << ">>";
                getline(cin, input);
                for (unsigned int i = 0; i < contents.size(); i++)
                {
                    if (input == contents[i]->getName())
                    {
                        item = contents[i];
                        finished = true;
                        break;
                    }
                }
                if (false == finished)
                {
                    cout << "Invalid option. Try again" << endl;
                }
            }
            if (location->takeItem(item) && player.takeItem(item))
            {
                cout << "Successfully taken item into players inventory!" << endl;
            }

        }
        else
        {
            cout << "The current location does not have any items!" << endl;
        }
    }
}

void dropMenu(Player& player)
{
    vector<Item*> inventory = player.getInventory();

    if (inventory.size() > 0)
    {
        Location* location = player.getLocation();
        Item* item = nullptr;
        string input;
        bool finished = false;
        cout << "Choose from one of the following items to drop" << endl;
        for (unsigned int i = 0; i < inventory.size(); i++)
        {
            cout << " - " << inventory[i]->getName() << endl;
        }

        while (false == finished)
        {
            cout << ">";
            getline(cin, input);
            for (unsigned int i = 0; i < inventory.size(); i++)
            {
                if (inventory[i]->getName() == input)
                {
                    item = inventory[i];
                    finished = true;
                    break;
                }
            }
            if (false == finished)
            {
                cout << "Invalid option. Try again" << endl;
            }
        }

        if (location && item)
        {
            if (player.hasItem(item))
            {
                player.dropItem(item);
                location->dropItem(item);
                cout << "Successfully dropped the item into the location" << endl;
            }
        }
    }
    else
    {
        cout << "Player Inventory is empty" << endl;
    }
}

void openMenu(Player& player)
{
    Location* location = player.getLocation();
    vector<Item*> contents;

    if (location)
    {
        contents = location->getContents();
        string line;
        bool finished = false;
        Item* item = nullptr;
        Item* key = nullptr;

        if (contents.size() > 0)
        {
            cout << "Select the item you would like to open" << endl;
            for (unsigned int i = 0; i < contents.size(); i++)
            {
                cout << " - " << contents[i]->getName() << endl;
            }
            while (false == finished)
            {
                cout << ">";
                getline(cin, line);
                for (unsigned int i = 0; i < contents.size(); i++)
                {
                    if (line == contents[i]->getName())
                    {
                        finished = true;
                        break;
                    }
                }
                if (false == finished)
                {
                    cout << "Invalid option. Try again" << endl;
                }
            }
            // If it is possible to open with the items I have on the inventory
            if (player.openItem(item))
            {
                cout << "Successfully opened item" << endl;
            }
            else
            {
                cout << "Not possible to open the item selected" << endl;
            }
        }
        else
        {
            cout << "There are not items to open in this location" << endl;
        }
    }
}

void moveMenu(Player& player)
{
    Location* location = player.getLocation();

    if (location)
    {
        vector<string> directions = location->getDirections();
        Location* newLocation = nullptr;
        string line;
        bool finished = false;

        cout << "Choose your direction:" << endl;
        for (unsigned int i = 0; i < directions.size(); i++)
        {
            cout << " - " << directions[i] << endl;
        }
        while (false == finished)
        {
            cout << ">";
            getline(cin, line);
            for (unsigned int i = 0; i < directions.size(); i++)
            {
                if (line == directions[i])
                {
                    finished = true;
                    break;
                }
            }
            if (false == finished)
            {
                cout << "Invalid option. Try again" << endl;
            }
        }
        newLocation = location->getConnection(line);
        if (newLocation)
        {
            player.setLocation(newLocation);
            cout << "Successfully moving to the new location" << endl;
        }
    }
}
