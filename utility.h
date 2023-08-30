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
inline int getFuzzyFourdirection(int r0, int c0, int r, int c)
{
    // 1 2 3
    // 4 5 6
    // 7 8 9
    // left: 4, right:6, up: 2, down: 8
    // x:col, y:row, return xy direction
    int dr = r - r0;
    int dc = c - c0;
    bool rowBigger = abs(dr) > abs(dc);
    if (rowBigger && dr > 0)
        return 8;
    if (rowBigger && dr < 0)
        return 2;
    if (!rowBigger && dc > 0)
        return 6;
    if (!rowBigger && dc < 0)
        return 4;
    return 5;
}
// template <typename T, typename... Args>
// void log(T first, Args... args)
// {
//     cout << first << endl;
//     log(args...);
// }
string description(int code);
#endif