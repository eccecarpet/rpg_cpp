#include <vector>
#include <iostream>
#include <fstream>
#include "GameFeatures.h"
#include "Container.h"


const vector<string> COMMANDS = { "LOOK", "TAKE", "DROP", "OPEN", "MOVE", "QUIT" };
//constant to be used to validate user input. Every char is related to one option in the UserAction enum

struct HelperStruct
{
    string name;
    string description;
    vector<string> contents;
};

struct LocationHelperStruct : HelperStruct
{
    int number;
    vector<string> connections;
};

struct ItemHelperStruct : HelperStruct
{
    string key;
};

string trimStartingSpaces(string input);
string toUpperCase(string input);
void parseContentsLine(const string& input, vector<string>& contents_vector);
void handleItemCreation(vector<Item*>& items, ItemHelperStruct* item_helper);
void handleLocationCreation(vector<Location*>& locations, vector<Item*> items, LocationHelperStruct* location_helper);

Command getUserAction()
{
    Command result = Command::Invalid;
    string input;
    bool validInput = false;

    // infinite loop until a valid selection is chosen
    while (!validInput)
    {
        cout << "Choose your move from the following options" << endl;
        for (unsigned int i = 0; i < COMMANDS.size(); i++)
        {
            cout << " - " << COMMANDS[i] << endl;
        }
        cout << ">>";
        getline(cin, input, '\n');
        input = toUpperCase(input);
        // iterates over all the valid options to check if it matches
        for (unsigned int i = 0; i < COMMANDS.size(); i++)
        {
            if (COMMANDS[i] == input)
            {
                validInput = true;
                result = static_cast<Command>(i);
                break;
            }
        }
        if (result == Command::Invalid)
        {
            cout << "Invalid option, try again" << endl;
        }
    }
    return result;
}

InputLineType getLineType(const string& line, bool isItem)
{
    InputLineType result = InputLineType::InvalidLine;
    if (line.empty())
    {
        result = InputLineType::EmptyLine;
    }
    else
    {
        string parsedLine = line.substr(0, line.find(':') + 1);
        if ("Item:" == parsedLine)
        {
            result = InputLineType::ItemHeader;
        }
        else if ("Keys:" == parsedLine)
        {
            result = InputLineType::ItemKeys;
        }
        else if ("Location:" == parsedLine)
        {
            result = InputLineType::LocationHeader;
        }
        else if ("Name:" == parsedLine)
        {
            result = InputLineType::LocationName;
        }
        else if ("Description:" == parsedLine)
        {
            if (isItem) result = InputLineType::ItemDescription;
            else result = InputLineType::LocationDescription;
        }
        else if ("Contents:" == parsedLine)
        {
            if (isItem) result = InputLineType::ItemContents;
            else result = InputLineType::LocationContents;
        }
        else
        {
            result = InputLineType::LocationConnections;
        }
    }
    return result;
}


void readData(vector<Item*>& items, vector<Location*>& locations, const string& filename)
{
    ifstream file;
    file.open(filename);

    if (file.is_open())
    {
        bool isItem = true;
        string line;
        ItemHelperStruct item_helper;
        LocationHelperStruct location_helper;
        vector<LocationHelperStruct> location_connections;

        while (getline(file, line))
        {
            InputLineType lineType = getLineType(line, isItem);

            switch (lineType)
            {
            case InputLineType::ItemHeader:
                //set name to the text after the : symbol
                item_helper.name = line.substr(line.find(':') + 1);
                item_helper.name = trimStartingSpaces(item_helper.name);
                break;
            case InputLineType::ItemDescription:
                item_helper.description = line.substr(line.find(':') + 1);
                item_helper.description = trimStartingSpaces(item_helper.description);
                break;
            case InputLineType::ItemContents:
                parseContentsLine(line, item_helper.contents);
                break;
            case InputLineType::ItemKeys:
                //set the key string to the text after the : symbol
                item_helper.key = line.substr(line.find(':') + 1);
                item_helper.key = trimStartingSpaces(item_helper.key);
                break;
            case InputLineType::LocationHeader:
                // all the items have been parsed now, so it must be time to parse the locations
                isItem = false;
                line = line.substr(line.find(':') + 1);
                location_helper.number = stoi(line);
                break;
            case InputLineType::LocationName:
                location_helper.name = line.substr(line.find(':') + 1);
                location_helper.name = trimStartingSpaces(location_helper.name);
                break;
            case InputLineType::LocationDescription:
                // set the description to the text after the : symbol
                location_helper.description = line.substr(line.find(':') + 1);
                location_helper.description = trimStartingSpaces(location_helper.description);
                break;
            case InputLineType::LocationContents:
                parseContentsLine(line, location_helper.contents);
                break;
            case InputLineType::LocationConnections:
                location_helper.connections.push_back(line);
                break;
            case InputLineType::EmptyLine:
                // if we are parsing the Items
                if (isItem)
                {
                    handleItemCreation(items, &item_helper);
                    //resets all the variables to be used in the next element description
                    item_helper.name = "";
                    item_helper.description = "";
                    item_helper.key = "";
                    item_helper.contents.clear();
                }
                else
                {
                    // parsing Locations
                    handleLocationCreation(locations, items, &location_helper);
                    location_connections.push_back(location_helper);
                    //resets all the variables to be used in the next element description
                    location_helper.name = "";
                    location_helper.description = "";
                    location_helper.number = 0;
                    location_helper.contents.clear();
                    location_helper.connections.clear();
                }
                break;
            default:
                break;
            }
        }
        file.close();

        // create the connections in the locations once all of the locations have been created & Iterate over all of the locations and make the connections
        for (unsigned int i = 0; i < location_connections.size(); i++)
        {
            vector<Location*> loc;
            vector<string> directions;
            int index = 0;
            int number = 0;
            vector<string>& connections = location_connections[i].connections;
            for (unsigned int j = 0; j < connections.size(); j++)
            {
                Location* next_location = nullptr;
                line = connections[j];
                string direction_string = line.substr(0, line.find(' '));

                // if the line contains any connection to other locations
                if (string::npos != line.find(' '))
                {
                    line = line.substr(line.find(' ') + 1);
                    if (line.find(',') != string::npos)
                    {
                        number = stoi(line.substr(line.find(',') - 1));
                    }
                    else
                    {
                        number = stoi(line);
                    }
                    index = findLocationIndex(locations, number);
                    if (index >= 0)
                    {
                        next_location = locations[index];
                    }
                }

              
                loc.push_back(next_location);
                directions.push_back(direction_string);
            }
            locations[i]->setConnections(loc);
            locations[i]->setDirections(directions);
        }
    }
}

string trimStartingSpaces(string input)
{
    int offset = 0;
    // finds for the offset that does not contain spaces
    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (input[i] != ' ')
        {
            break;
        }
        offset++;
    }
    return input.substr(offset);
}

string toUpperCase(string input)
{
    string result;
    for (unsigned int i = 0; i < input.size(); i++)
    {
        result.push_back(::toupper(input[i]));
    }
    return result;
}

int findItemIndex(vector<Item*> items, string name)
{
    int result = -1;

    for (unsigned int i = 0; i < items.size(); i++)
    {
        if (items[i]->getName() == name)
        {
            result = i;
            break;
        }
    }

    return result;
}

int findLocationIndex(vector<Location*> locations, int number)
{
    int result = -1;
    for (unsigned int i = 0; i < locations.size(); i++)
    {
        if (locations[i]->getNumber() == number)
        {
            result = i;
            break;
        }
    }
    return result;
}

void parseContentsLine(const string& input, vector<string>& contents_vector)
{
    bool finishedParsingLine = false;
    string contents;
   
    string currentLine = input.substr(input.find(':') + 1);
    // iterates on the line to add all the contents in the vector
    while (!finishedParsingLine)
    {
        // if we found a comma in the text, means there shall be more contents remaining
        if (string::npos != currentLine.find(','))
        {
            contents = currentLine.substr(0, currentLine.find(','));
            contents = trimStartingSpaces(contents);
            contents_vector.push_back(contents);
            // will change the line from 'Red Key, Letter' to ' Letter'
            currentLine = currentLine.substr(currentLine.find(',') + 1);
        }
        else
        {
            currentLine = trimStartingSpaces(currentLine);
            // If there is an element on the line, add it to the vector
            // In case the Item does not have any Contents, this will be empty,
            // therefore no need to add into the vector
            if (currentLine.size() > 0)
            {
                contents_vector.push_back(currentLine);
            }
            finishedParsingLine = true;
        }
    }
}

void handleItemCreation(vector<Item*>& items, ItemHelperStruct* item_helper)
{
    // meaning it is a Container because it has Contents: and Keys:
    if (!item_helper->contents.empty())
    {
        int indexItem = -1;
        vector<Item*> contents_vector;
        // iterates over all the names of the content and find the pointer to it
        for (unsigned int i = 0; i < item_helper->contents.size(); i++)
        {
            // if the Item was already created, findItemIndex will return the index to the correct ptr
            indexItem = findItemIndex(items, item_helper->contents[i]);
            if (indexItem >= 0)
            {
                // only add to the items vector if the item exists
                contents_vector.push_back(items[indexItem]);
            }
        }
        Container* container = nullptr;
        indexItem = findItemIndex(items, item_helper->key);

        // only if the key item exists add it to the new container, otherwise set it to nullptr
        if (indexItem >= 0)
        {
            container = new Container(item_helper->name,
                item_helper->description,
                contents_vector,
                items[indexItem]);
        }
        else
        {
            container = new Container(item_helper->name,
                item_helper->description,
                contents_vector,
                nullptr);
        }
        // push the container to the items vector
        items.push_back(container);
    }
    else
    {
        Item* item = new Item(item_helper->name, item_helper->description);
        items.push_back(item);
    }
}

void handleLocationCreation(vector<Location*>& locations, vector<Item*> items, LocationHelperStruct* location_helper)
{
    vector<Item*> location_items;

    for (unsigned int i = 0; i < location_helper->contents.size(); i++)
    {
        int index = -1;

        index = findItemIndex(items, location_helper->contents[i]);
        if (index >= 0)
        {
            location_items.push_back(items[index]);
        }
    }
    Location* loc = new Location(location_helper->name, location_helper->description, location_helper->number);
    loc->setContents(location_items);
    locations.push_back(loc);
}
