#include "utility.h"
#include <memory>
#include "units.h"
#include <string>
void loadSmallMap(unique_ptr<pair<int, int>[]> &map)
{
    int size = 9;
    for (int i = 0; i < size * size; i++)
    {
        map[i].first = 10;
        map[i].second = 0; // no character
    }
    map[4].second = 2;        // red kn
    map[size + 1].second = 1; // red ft
    map[size + 7].second = 1; // red ft
    for (int i = 0; i < 3; i++)
    {
        map[size + 3 + i].first = 14; // forest
    }
    map[2 * size + 1].first = 12;  // moutain
    map[2 * size + 7].first = 12;  // moutain
    map[4 * size].first = 11;      // water
    map[4 * size + 2].first = 11;  // water
    map[4 * size + 3].first = 11;  // water
    map[4 * size + 5].first = 11;  // water
    map[4 * size + 6].first = 11;  // water
    map[4 * size + 8].first = 11;  // water
    map[8 * size + 4].second = -2; // blue kn
    map[7 * size + 1].second = -1; // blue ft
    map[7 * size + 7].second = -1; // blue ft
    for (int i = 0; i < 3; i++)
    {
        map[6 * size + 3 + i].first = 14; // forest
    }
    map[6 * size + 1].first = 12; // moutain
    map[6 * size + 7].first = 12; // moutain
    map[5 * size + 5].second = 4; // test mage
}
void loadEmptyMap(unique_ptr<pair<int, int>[]> &map, int mapSize)
{
    for (int i = 0; i < mapSize; ++i)
    {
        map[i].first = 10;
        map[i].second = 0;
    }
}
bool isMapChanged(pair<int, int> *oldMap, pair<int, int> *newMap, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (oldMap[i] != newMap[i])
            return true;
    }
    return false;
}
void arrayCopy(pair<int, int> *source, pair<int, int> *target, int size)
{
    for (int i = 0; i < size; ++i)
    {
        target[i] = source[i];
    }
}

int m_dis(int x0, int y0, int x1, int y1)
{
    return abs(x0 - x1) + abs(y0 - y1);
}
string description(int code)
{
    // mask code: 0-9 characters, 10-19 terrain, 20-39 user_input, 40-59 attached-effects
    // 1 red_ft, 2 red_kn, 3 red_ar, 4 red_mg
    // -1, -2, -3, -4 blue
    // 10 plain, 11 water, 12 mountains, 13 abyss, 19 fire, 14 forest
    // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack
    string display;
    switch (code)
    {
    case 1:
        display = "A red footman";
        break;
    case 2:
        display = "A red knight";
        break;
    case 3:
        display = "A red archer";
        break;
    case 4:
        display = "A red mage";
        break;
    case -1:
        display = "A blue footman";
        break;
    case -2:
        display = "A blue knight";
        break;
    case -3:
        display = "A blue archer";
        break;
    case -4:
        display = "A blue mage";
        break;
    case 10:
        display = "plain area";
        break;
    case 11:
        display = "deep water";
        break;
    case 13:
        display = "abyss";
        break;
    case 12:
        display = "mountains";
        break;
    case 14:
        display = "forest";
        break;
    case 19:
        display = "This area is onfire";
        break;
    case 20:
        display = "fireball";
        break;
    case 21:
        display = "earthquake";
        break;
    default:
        display = "?";
        break;
    }
    return display;
}