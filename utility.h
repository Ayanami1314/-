#ifndef GAME_UTILITY_
#define GAME_UTILITY_
#include <memory>
using namespace std;
void loadSmallMap(unique_ptr<pair<int, int>[]> &map);
void loadEmptyMap(unique_ptr<pair<int, int>[]> &map, int mapSize);
bool isMapChanged(pair<int, int> *oldMap, pair<int, int> *newMap, int size);
void arrayCopy(pair<int, int> *source, pair<int, int> *target, int size);
int m_dis(int x0, int y0, int x1, int y1);
string description(int code);
#endif