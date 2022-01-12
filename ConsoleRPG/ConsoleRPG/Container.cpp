#include "Container.h"



Container::Container(const string& name, const string& description,
    vector<Item*> contents, Item* kItem) :
    Item(name, description)
{
    Container::contents = contents;
    Container::keyItem = kItem;
    Container::opened = false;
}

bool Container::open(Item* item)
{
    // If the pointer is valid, the container has not been opened before and the item used is the one we need
    if (item && !opened && *item == *keyItem)
    {
        opened = true;
    }
    return false;
}

Item* Container::getKeyItem() const
{
    return keyItem;
}

vector<Item*>& Container::getContents()
{
    return contents;
}
