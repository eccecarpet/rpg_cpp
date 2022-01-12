#pragma once
#include <string>
#include <vector>
#include "Item.h"

using namespace std;

struct Location
{
    Location(std::string name, std::string description, int number);
    int getNumber() const;
    std::string getName() const;
    std::string getDescription() const;
    void print() const;
    bool takeItem(Item* item);
    void dropItem(Item* i);
    Item* getKey(string& direction);
    Location* getConnection(string& direction);
    vector<Item*> getContents() const;
    vector<string> getDirections() const;
    void setContents(vector<Item*> contents);
    void setConnections(vector<Location*> connections);
    void setDirections(vector<string> directions);
private:
    int number;
    std::string name;
    std::string description;
    vector<Item*> contents;
    vector<Item*> keyItems;
    vector<Location*> connections;
    vector<string> directions;
};
