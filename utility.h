#ifndef GAME_UTILITY_
#define GAME_UTILITY_
#include <memory>
using namespace std;
void loadSmallMap(unique_ptr<int[]> &map);
void loadEmptyMap(unique_ptr<int[]> &map, int mapSize);
bool isMapChanged(int *oldMap, int *newMap, int size);
void arrayCopy(int *source, int *target, int size);
#endif