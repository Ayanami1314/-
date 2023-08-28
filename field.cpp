#include <string>
#include <cassert>
#include <iomanip>
#include "field.h"
#include "units.h"
#include <memory>
using namespace std;

/* Battle field */

// Constructor
Field::Field(int h, int w)
    : units(h, w), terrains(h, w)
{
    assert(h > 0 && h <= 20 && w > 0 && w <= 20);
}

// Get the height of the field
int Field::getHeight() const
{
    return units.numRows();
}

// Get the width of the field
int Field::getWidth() const
{
    return units.numCols();
}

// Get the terrain of a square
Terrain Field::getTerrain(int row, int col) const
{
    assert(terrains.inBounds(row, col));

    return terrains[row][col];
}
Unit *Field::getUnit(int row, int col) const
{
    assert(units.inBounds(row, col));
    return units[row][col];
}
// Reclaim all the units
Field::~Field()
{
    for (int i = 0; i < units.numRows(); i++)
        for (int j = 0; j < units.numCols(); j++)
            if (units[i][j] != nullptr)
                delete units[i][j];
}

// Display the symbol for terrains
string getTerrainSymbol(Terrain t)
{
    switch (t)
    {
    case PLAIN:
        return "  ";
        break;
    case MOUNTAIN:
        return "/\\";
        break;
    case WATER:
        return "~~";
        break;
    case FOREST:
        return "$ ";
        break;
    case ABYSS:
        return "\\/";
        break;
    case ONFIRE:
        return "@@";
        break;
    default:
        break;
    }
    // assert(false);
    return "  ";
}
string getUnitSymbol(Unit *u)
{
    switch (u->getType())
    {
    case FOOTMAN:
        return "FT";
        break;
    case KNIGHT:
        return "KN";
        break;
    case ARCHER:
        return "AR";
        break;
    case MAGE:
        return "MG";
        break;
    default:
        return "  ";
    }
}
// Print the horizontal line
void printHLine(ostream &os, int n)
{
    os << "  ";
    for (int i = 0; i < n; i++)
        os << "+--";
    os << "+" << endl;
}

// Display field
ostream &operator<<(ostream &os, const Field &field)
{
    int height = field.getHeight();
    int width = field.getWidth();

    // Print the x coordinates
    os << "  ";
    for (int i = 0; i < width; i++)
        os << ' ' << setw(2) << i;
    os << endl;

    printHLine(os, width);
    for (int i = 0; i < height; i++)
    {
        os << setw(2) << i;
        for (int j = 0; j < width; j++)
        {
            os << '|';
            string ts = getTerrainSymbol(field.getTerrain(i, j));
            string us = getUnitSymbol(field.getUnit(i, j));
            bool sd = field.getUnit(i, j)->getSide();
            if (us != "  ")
            {
                if (!sd)
                {
                    for (int k = 0; k < us.size(); k++)
                    {
                        us[k] = us[k] - 'A' + 'a';
                    }
                }
                os << us;
            }
            else
            {
                os << ts;
            }
        }
        os << '|' << endl;
        printHLine(os, width);
    }
    os << endl;
    return os;
}
bool Field::moveUnit(int r0, int c0, int r, int c)
{
    Unit *u = this->getUnit(r0, c0);
    if (r < 0 || r >= this->getHeight() || c < 0 || c >= this->getWidth())
        return false;
    if (this->getUnit(r, c)->getType() != UNDEFINED)
        return false;
    Unit *target = this->getUnit(r, c);
    target->setType(u->getType());
    target->setSide(u->getSide());
    u->setType(UNDEFINED);
    return true;
}

bool Field::inBounds(int row, int col) const
{
    if (row < 0 || row >= this->getHeight() || col < 0 || col >= this->getWidth())
    {
        return false;
    }
    return true;
}
void Field::loadmap_array(pair<int, int> *map, int mapSize)
{
    // mask code: 0-9 characters, 10-19 terrain, 20-39 user_input, 40-59 attached-effects
    // 1 red_ft, 2 red_kn, 3 red_ar, 4 red_mg
    // -1, -2, -3, -4 blue
    // 10 plain, 11 water, 12 mountains, 13 abyss, 19 fire, 14 forest
    // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            switch (map[i * mapSize + j].second)
            {
            // true: red, false: blue
            case 1:
                this->units[i][j] = new Unit(FOOTMAN, true);
                break;
            case 2:
                this->units[i][j] = new Unit(KNIGHT, true);
                break;
            case 3:
                this->units[i][j] = new Unit(ARCHER, true);
                break;
            case 4:
                this->units[i][j] = new Unit(MAGE, true);
                break;
            case -1:
                this->units[i][j] = new Unit(FOOTMAN, false);
                break;
            case -2:
                this->units[i][j] = new Unit(KNIGHT, false);
                break;
            case -3:
                this->units[i][j] = new Unit(ARCHER, false);
                break;
            case -4:
                this->units[i][j] = new Unit(MAGE, false);
                break;
            default:
                this->units[i][j] = new Unit(UNDEFINED, false);
                break;
            }
            switch (map[i * mapSize + j].first)
            {
            // true: red, false: blue
            case 10:
                this->setTerrain(i, j, PLAIN);
                break;
            case 12:
                this->setTerrain(i, j, MOUNTAIN);
                break;
            case 11:
                this->setTerrain(i, j, WATER);
                break;
            case 13:
                this->setTerrain(i, j, ABYSS);
                break;
            case 14:
                this->setTerrain(i, j, FOREST);
                break;
            case 19:
                this->setTerrain(i, j, ONFIRE);
                break;
            default:
                this->setTerrain(i, j, PLAIN);
                break;
            }
        }
    }
}
void Field::updateArray(pair<int, int> *mapArray, int size)
{
    // call it when field changes to update mapArray
    // the reverse map of loadmap_array, use the same code set
    auto reverseMapTerrain = [](Terrain t) -> int
    {
        switch (t)
        {
        case PLAIN:
            return 10;
            break;
        case MOUNTAIN:
            return 12;
            break;
        case WATER:
            return 11;
            break;
        case ABYSS:
            return 13;
            break;
        case FOREST:
            return 14;
            break;
        case ONFIRE:
            return 19;
            break;
        default:
            return 10;
            break;
        }
    };
    auto reverseMapUnit = [](Unit *pUnit) -> int
    {
        UnitType ut = pUnit->getType();
        bool sd = pUnit->getSide();
        int code = 0;
        switch (ut)
        {
        case UNDEFINED:
            code = 0;
            break;
        case FOOTMAN:
            code = 1;
            break;
        case KNIGHT:
            code = 2;
            break;
        case ARCHER:
            code = 3;
            break;
        case MAGE:
            code = 4;
            break;
        default:
            code = 0;
            break;
        }
        return sd ? code : -code;
    };
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {

            mapArray[i * size + j].first = reverseMapTerrain(this->getTerrain(i, j));
            mapArray[i * size + j].second = reverseMapUnit(this->getUnit(i, j));
        }
    }
}