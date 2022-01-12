#pragma once
#include <string>
#include <vector>
#include "Item.h"

using namespace std;

class Container : public Item
{
public:
    Container(const string& name, const string& description, vector<Item*> contents, Item* keyItem);

    bool open(Item* item) override;
    Item* getKeyItem() const override;
    vector<Item*>& getContents() override;


private:

    vector<Item*> contents;
    Item* keyItem;
    bool opened;
};
