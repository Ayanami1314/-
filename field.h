#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <ostream>
#include "grid.h"
using std::pair;
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
    void loadmap_array(pair<int, int> *map, int mapSize, int h, int w);
    friend void createFireBall(Field *f, int r, int c, int d);
    friend void createEarthquake(Field *f, int r, int c, int d);
    friend void Flow(Field *f);
    bool canOver(int r, int c) const
    {
        return this->terrains[r][c] != WATER && this->terrains[r][c] != MOUNTAIN && this->terrains[r][c] != ABYSS;
    };
    // Constructor
    Field(int h, int w);
    bool moveUnit(int r0, int c0, int r, int c);
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
    void setUnit(int r, int c, Unit *u)
    {
        units[r][c] = u;
    }
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
