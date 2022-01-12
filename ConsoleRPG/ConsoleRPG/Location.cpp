#include <iostream>
#include <string>
#include "Location.h"
using namespace std;

Location::Location(string name, string description, int number)
{
    Location::name = name;
    Location::description = description;
    Location::number = number;
}

std::string Location::getName() const
{
    return name;
}

std::string Location::getDescription() const
{
    return description;
}

int Location::getNumber() const
{
    return number;
}

void Location::print() const
{
    cout << "Location: " << getName() << ". " << getDescription() << endl;
    cout << "Contents:" << endl;
    for (unsigned int i = 0; i < contents.size(); i++)
    {
        cout << " - " << contents[i]->getName() << endl;
    }
}

bool Location::takeItem(Item* item)
{
    bool result = false;
    int index = -1;
    //iterates over all of the contents of the location
    for (unsigned int i = 0; i < contents.size(); i++)
    {
        if (item == contents[i])
        {
            // when item is found, set the index and exit the loop
            index = i;
            break;
        }
    }

    //if the item was in the contents of the vector
    if (index >= 0)
    {
        //create a new vector without the element to be deleted
        vector<Item*> newVector;
        for (unsigned int i = 0; i < contents.size(); i++)
        {
            if (i != index)
            {
                newVector.push_back(contents[i]);
            }
        }
        contents = newVector;
        result = true;
    }
    return result;
}

void Location::dropItem(Item* i)
{
    //adds the item into the contents vector
    contents.push_back(i);
}

Item* Location::getKey(string& direction)
{
    Item* result = nullptr;
    int index = -1;

    //iterate over all of the connections of the location to find the direction the user is asking
    for (unsigned int i = 0; i < connections.size(); i++)
    {
        //direction is found
        if (direction == connections[i]->getName())
        {
            //set index and exit the loop
            index = i;
            break;
        }
    }

    //if we found the direction, see if the keyItems has a requirement
    // keItems and connections has a one-to-one relation where the index of the connection corresponds to the index of its keyItems requirement
    
    if (index >= 0)
    {
        result = keyItems[index];
    }

    return result;
}

Location* Location::getConnection(string& direction)
{
    Location* result = nullptr;

    // iterates over all of the connections trying to find the direction and if found, sets the pointer to the corresponding value
    for (unsigned int i = 0; i < directions.size(); i++)
    {
        if (direction == directions[i])
        {
            //direction found, set pointer and exit the loop
            result = connections[i];
            break;
        }
    }
    return result;
}

void Location::setContents(vector<Item*> contents)
{
    Location::contents = contents;
}

void Location::setConnections(vector<Location*> connections)
{
    Location::connections = connections;
}

void Location::setDirections(vector<string> directions)
{
    Location::directions = directions;
}

vector<Item*> Location::getContents() const
{
    return contents;
}

vector<string> Location::getDirections() const
{
    return directions;
}
