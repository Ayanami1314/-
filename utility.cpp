#include "utility.h"
#include <memory>
void loadSmallMap(unique_ptr<int[]> &map)
{
    int size = 9;
    for (int i = 0; i < size * size; i++)
    {
        map[i] = 10;
    }
    map[4] = 2;        // red kn
    map[size + 1] = 1; // red ft
    map[size + 7] = 1; // red ft
    for (int i = 0; i < 3; i++)
    {
        map[size + 3 + i] = 14; // forest
    }
    map[2 * size + 1] = 12; // moutain
    map[2 * size + 7] = 12; // moutain
    map[4 * size] = 11;     // water
    map[4 * size + 2] = 11; // water
    map[4 * size + 3] = 11; // water
    map[4 * size + 5] = 11; // water
    map[4 * size + 6] = 11; // water
    map[4 * size + 8] = 11; // water
    map[8 * size + 4] = -2; // blue kn
    map[7 * size + 1] = -1; // blue ft
    map[7 * size + 7] = -1; // blue ft
    for (int i = 0; i < 3; i++)
    {
        map[6 * size + 3 + i] = 14; // forest
    }
    map[6 * size + 1] = 12; // moutain
    map[6 * size + 7] = 12; // moutain
}
void loadEmptyMap(unique_ptr<int[]> &map, int mapSize)
{
    for (int i = 0; i < mapSize; ++i)
    {
        map[i] = 10;
    }
}
bool isMapChanged(int *oldMap, int *newMap, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (oldMap[i] != newMap[i])
            return true;
    }
    return false;
}
void arrayCopy(int *source, int *target, int size)
{
    for (int i = 0; i < size; ++i)
    {
        target[i] = source[i];
    }
}