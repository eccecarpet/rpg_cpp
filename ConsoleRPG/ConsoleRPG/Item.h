#pragma once

#include <string>
#include <vector>

using namespace std;

class Item
{
public:
    Item(const string& name, const string& description);
    virtual ~Item();
    const string& getName() const;
    const string& getDescription() const;
    bool operator==(const Item& other) const;

    // defining child classes functions
    virtual bool open(Item* i);
    virtual Item* getKeyItem() const;
    virtual vector<Item*>& getContents();
protected:
    string name;
    string description;
};
