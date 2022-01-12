#include "Item.h"

Item::Item(const string& name, const string& description)
{
    Item::name = name;
    Item::description = description;
}

const string& Item::getName() const
{
    return name;
}

const string& Item::getDescription() const
{
    return description;
}

bool Item::operator==(const Item& other) const
{
    return (name == other.name && description == other.description);
}

bool Item::open(Item* i)
{
    // only child classes can open an item, therefore this function need to be overridden
    return false;
}

Item* Item::getKeyItem() const
{
    return nullptr;
}

vector<Item*>& Item::getContents()
{
    static vector<Item*> emptyContents;
    return emptyContents;
}

Item::~Item()
{
}
