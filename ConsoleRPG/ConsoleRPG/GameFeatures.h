#pragma once
#include <string>
#include "Location.h"

//using namespace std;

// enum used to handle the user input options
enum class Command
{
    LOOK = 0,
    TAKE,
    DROP,
    OPEN,
    MOVE,
    QUIT,
    Invalid,
};

//enum used to identify from the input file what type of line it is and what information it contains
enum class InputLineType
{
    ItemHeader = 0,
    ItemDescription,
    ItemContents,
    ItemKeys,
    LocationHeader,
    LocationName,
    LocationDescription,
    LocationContents,
    LocationConnections,
    EmptyLine,
    InvalidLine
};

void readData(vector<Item*>& items, vector<Location*>& locations, const string& filename);
Command getUserAction();
InputLineType getLineType(const std::string& line, bool isItem);

int findItemIndex(vector<Item*> items, string name);
int findLocationIndex(vector<Location*> locations, int number);

