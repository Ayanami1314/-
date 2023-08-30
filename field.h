#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <ostream>
#include "grid.h"
#include "units.h"
#include <vector>
using std::pair;
using std::vector;
/* Terrains */
enum Terrain
{
    PLAIN,
    MOUNTAIN,
    FOREST,
    WATER,
    ABYSS,
    ONFIRE
};
// Forward declaration of the class of units
class Unit;
/* Battle field */
class Field
{
public:
    friend Field *loadmap(std::istream &is);
    //  GUI's backend
    void loadmap_array(pair<int, int> *map, int mapSize);
    friend void createFireBall(Field *f, int r, int c, int d);
    friend void createEarthquake(Field *f, int r, int c, int d);
    friend void Flow(Field *f);
    void refreshTheState(); // new turn
    void burnOut()
    {
        for (int i = 0; i < this->getHeight(); i++)
        {
            for (int j = 0; j < this->getWidth(); j++)
            {
                if (this->terrains[i][j] == ONFIRE)
                {
                    this->terrains[i][j] = PLAIN;
                }
            }
        }
    }
    void updateArray(pair<int, int> *mapArray, int size);
    bool canOver(int r, int c) const
    {
        bool terrainFlag = this->terrains[r][c] != WATER && this->terrains[r][c] != MOUNTAIN && this->terrains[r][c] != ABYSS && this->terrains[r][c] != ONFIRE;
        bool unitFlag = this->units[r][c] != nullptr && this->units[r][c]->getType() == UNDEFINED;
        return terrainFlag && unitFlag;
    };
    // Constructor
    Field(int h, int w);
    bool moveUnit(int r0, int c0, int r, int c, int &remainSteps);
    bool attack(int r0, int c0, int r, int c, int &remainSteps);
    vector<pair<int, int>> getCanAttackBlocks(int r0, int c0);
    vector<pair<int, int>> getCanMoveBlocks(int r0, int c0, int remainSteps);
    vector<pair<int, int>> getFireBallBlocks(int r0, int c0);
    vector<pair<int, int>> getEarthquakeBlocks(int r0, int c0);
    // Get the height and width of the field
    int getHeight() const;
    int getWidth() const;
    // Get the terrain
    Terrain getTerrain(int row, int col) const;
    void onFire(int r, int c)
    {
        terrains[r][c] = ONFIRE;
    }
    void setTerrain(int r, int c, Terrain t)
    {
        terrains[r][c] = t;
    }
    void setUnit(int r, int c, UnitType ut, bool side, int movePoints = -1)
    {
        if (units[r][c] == nullptr && movePoints == -1)
        {
            units[r][c] = new Unit(ut, side);
        }
        if (units[r][c] == nullptr && movePoints != -1)
        {
            units[r][c] = new Unit(ut, side, movePoints);
        }
        if (movePoints != -1)
        {
            // -1: default movePoint of the Type
            units[r][c]->setType(ut);
            units[r][c]->setSide(side);
            units[r][c]->setMovePoints(movePoints);
        }
        else
        {
            units[r][c]->setType(ut);
            units[r][c]->setSide(side);
            units[r][c]->setMovePointsByType(ut);
        }
    }
    void setUnit(int r, int c, Unit *u)
    {
        units[r][c] = u;
    }
    void setUnitMagicNumber(int r, int c, int magicNumber);
    Unit *getUnit(int row, int col) const;
    bool inBounds(int row, int col) const;
    // Destructor
    ~Field();

private:
    // Store the units
    Grid<Unit *> units;
    // Store the terrain
    Grid<Terrain> terrains;
};

// Display the field on the out stream os
std::ostream &operator<<(std::ostream &os, const Field &field);

#endif // FIELD_H_INCLUDED
