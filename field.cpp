#include <string>
#include <cassert>
#include <iomanip>
#include "field.h"
#include "units.h"
using namespace std;

/* Battle field */

// Constructor
Field::Field(int h, int w)
    : units(h, w), terrains(h, w) {
    assert(h > 0 && h <= 20 && w > 0 && w <= 20);

}

// Get the height of the field
int Field::getHeight() const {
    return units.numRows();
}

// Get the width of the field
int Field::getWidth() const {
    return units.numCols();
}

// Get the terrain of a square
Terrain Field::getTerrain(int row, int col) const {
    assert(terrains.inBounds(row, col));

    return terrains[row][col];
}
Unit* Field::getUnit(int row, int col) const {
    assert(units.inBounds(row, col));
    return units[row][col];
}
// Reclaim all the units
Field::~Field() {
    for (int i = 0; i < units.numRows(); i++)
        for (int j = 0; j < units.numCols(); j++)
            if (units[i][j] != nullptr)
                delete units[i][j];
}



// Display the symbol for terrains
string getTerrainSymbol(Terrain t) {
    switch (t)
    {
        case PLAIN:
            return "  "; break;
        case MOUNTAIN:
            return "/\\"; break;
        case WATER:
            return "~~"; break;
        case FOREST:
            return "$ "; break;
        case ABYSS:
            return "\\/"; break;
        case ONFIRE:
            return "@@"; break;
        default:
            break;
    }
    //assert(false);
    return "  ";
}
string getUnitSymbol(Unit* u) {
    switch (u->getType())
    {
        case FOOTMAN: return "FT"; break;
        case KNIGHT: return "KN"; break;
        case ARCHER: return "AR"; break;
        case MAGE:return "MG"; break;
        default: return "  ";
    }
}
// Print the horizontal line
void printHLine(ostream& os, int n) {
    os << "  ";
    for (int i = 0; i < n; i++)
        os << "+--";
    os << "+" << endl;
}

// Display field
ostream& operator<<(ostream& os, const Field& field) {
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
bool Field::moveUnit(int r0, int c0, int r, int c) {
    Unit* u = this->getUnit(r0, c0);
    if (r < 0 || r >= this->getHeight() || c < 0 || c >= this->getWidth())
        return false;
    if (this->getUnit(r, c)->getType() != UNDEFINED)
        return false;
    Unit* target = this->getUnit(r, c);
    target->setType(u->getType());
    target->setSide(u->getSide());
    u->setType(UNDEFINED);
    return true;
}

bool Field::inBounds(int row, int col) const {
    if(row < 0 || row >= this->getHeight() || col < 0 || col >= this->getWidth()){
        return false;
    }
    return true;
}
