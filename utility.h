#ifndef GAME_UTILITY_
#define GAME_UTILITY_
#include <memory>
#include "units.h"
using namespace std;
void loadSmallMap(unique_ptr<pair<int, int>[]> &map);
void loadEmptyMap(unique_ptr<pair<int, int>[]> &map, int mapSize);
bool isMapChanged(pair<int, int> *oldMap, pair<int, int> *newMap, int size);
void arrayCopy(pair<int, int> *source, pair<int, int> *target, int size);
int m_dis(int x0, int y0, int x1, int y1);
inline bool isCharacter(int code)
{
    return abs(code) < 10 && code != 0;
};
inline bool isTerrain(int code)
{
    return code > 10 && code < 20;
}
inline int getMoveSteps(UnitType ut)
{
    // get steps this unit can move
    switch (ut)
    {
    case FOOTMAN:
        return 3;
    case KNIGHT:
        return 4;
    case ARCHER:
        return 2;
    case MAGE:
        return 2;
    default:
        return 0;
    }
    return 0;
}

string description(int code);
#endif