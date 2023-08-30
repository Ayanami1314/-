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
    // units.fill(new Unit());  THE SOURCE OF ERROR!!!!!!!!! fill() only copy the value, which means A pointer!!!!!
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            units[i][j] = new Unit();
        }
    }
    terrains.fill(Terrain::PLAIN);
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
bool Field::moveUnit(int r0, int c0, int r, int c, int &remainSteps)
{

    Unit *u = this->getUnit(r0, c0);
    if (r < 0 || r >= this->getHeight() || c < 0 || c >= this->getWidth())
        return false;
    if (this->getUnit(r, c)->getType() != UNDEFINED)
        return false;
    if (!canOver(r, c))
        return false;
    // bfs
    deque<pair<int, int>> q;
    q.push_back(make_pair(r0, c0));
    vector<vector<bool>> visited(this->getHeight(), vector<bool>(this->getWidth(), false));
    visited[r0][c0] = true;
    int INF = 100000000;
    vector<vector<int>> distance(this->getHeight(), vector<int>(this->getWidth(), INF));
    distance[r0][c0] = 0;
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop_front();
        for (int i = 0; i < 4; i++)
        {
            int nr = p.first + directions[i][0];
            int nc = p.second + directions[i][1];
            if (nr >= 0 && nr < this->getHeight() && nc >= 0 && nc < this->getWidth() && !visited[nr][nc] && this->canOver(nr, nc))
            {
                q.push_back(make_pair(nr, nc));
                visited[nr][nc] = true;
                distance[nr][nc] = distance[p.first][p.second] + 1;
            }
        }
        if (distance[p.first][p.second] != INF && distance[p.first][p.second] > remainSteps)
        {
            cout << "remainSteps:" << remainSteps << endl;
            break;
        }
    }
    cout << "distance" << endl;
    for (int i = 0; i < this->getHeight(); i++)
    {
        for (int j = 0; j < this->getWidth(); j++)
        {
            if (distance[i][j] == INF)
            {
                cout << "I ";
            }
            else
            {
                cout << distance[i][j] << " ";
            }
        }
        cout << endl;
    }
    if (remainSteps < distance[r][c])
    {
        cout << "can't move!" << endl;
        return false;
    }
    this->setUnit(r, c, u->getType(), u->getSide(), u->getMovePoints() - distance[r][c]);
    this->setUnit(r0, c0, UNDEFINED, false);
    remainSteps -= distance[r][c];
    return true;
}
vector<pair<int, int>> Field::getCanMoveBlocks(int r0, int c0, int remainSteps)
{
    deque<pair<int, int>> q;
    q.push_back(make_pair(r0, c0));
    vector<vector<bool>> visited(this->getHeight(), vector<bool>(this->getWidth(), false));
    visited[r0][c0] = true;
    int INF = 100000000;
    vector<vector<int>> distance(this->getHeight(), vector<int>(this->getWidth(), INF));
    distance[r0][c0] = 0;
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    vector<pair<int, int>> canMoveBlocks;
    while (!q.empty())
    {
        pair<int, int> p = q.front();
        q.pop_front();
        for (int i = 0; i < 4; i++)
        {
            int nr = p.first + directions[i][0];
            int nc = p.second + directions[i][1];
            if (nr >= 0 && nr < this->getHeight() && nc >= 0 && nc < this->getWidth() && !visited[nr][nc] && this->canOver(nr, nc))
            {
                q.push_back(make_pair(nr, nc));
                visited[nr][nc] = true;
                distance[nr][nc] = distance[p.first][p.second] + 1;
            }
        }
        if (distance[p.first][p.second] <= remainSteps)
        {
            canMoveBlocks.push_back(p);
        }
        else if (distance[p.first][p.second] != INF)
        {
            break;
        }
    }
    return canMoveBlocks;
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
                this->setUnit(i, j, FOOTMAN, true);
                break;
            case 2:
                this->setUnit(i, j, KNIGHT, true);
                break;
            case 3:
                this->setUnit(i, j, ARCHER, true);
                break;
            case 4:
                this->setUnit(i, j, MAGE, true);
                break;
            case -1:
                this->setUnit(i, j, FOOTMAN, false);
                break;
            case -2:
                this->setUnit(i, j, KNIGHT, false);
                break;
            case -3:
                this->setUnit(i, j, ARCHER, false);
                break;
            case -4:
                this->setUnit(i, j, MAGE, false);
                break;
            default:
                this->setUnit(i, j, UNDEFINED, false);
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
bool Field::attack(int r0, int c0, int r, int c, int &remainSteps)
{
    int MapHeight = this->getHeight();
    int MapWidth = this->getWidth();
    vector<pair<int, int>> canAtkBlocks = getCanAttackBlocks(r0, c0);
    if (canAtkBlocks.size() == 0)
    {
        return false;
    }
    UnitType ut = this->getUnit(r0, c0)->getType();
    bool sd = this->getUnit(r0, c0)->getSide();
    for (auto &p : canAtkBlocks)
    {
        if (p.first == r && p.second == c)
        {
            remainSteps = 0;
            this->setUnit(r0, c0, ut, sd, 0);        // clear the movePoints
            this->setUnit(r, c, UNDEFINED, true, 0); // kill the target unit
            return true;
        }
    }
    return false;
}
vector<pair<int, int>> Field::getCanAttackBlocks(int r0, int c0)
{
    vector<pair<int, int>> directions;
    vector<pair<int, int>> canAttackBlocks;
    // directions: pair<direction, distance>, direction is 1~9, distance is 1, 2,...
    // 1 2 3
    // 4 u 6
    // 7 8 9
    int MapHeight = this->getHeight();
    int MapWidth = this->getWidth();
    if (this->getUnit(r0, c0) == nullptr || this->getUnit(r0, c0)->getType() == UNDEFINED)
    {
        return canAttackBlocks;
    }
    UnitType ut = this->getUnit(r0, c0)->getType();
    bool sd = this->getUnit(r0, c0)->getSide();
    switch (ut)
    {
    case FOOTMAN:
    {
        for (int i = 1; i <= 9; i++)
        {
            directions.push_back(make_pair(i, 1));
        }
        break;
    }
    case KNIGHT:
    {
        for (int i = 1; i <= 9; i++)
        {
            directions.push_back(make_pair(i, 1));
        }
        break;
    }
    case ARCHER:
    {
        for (int i = 1; i <= 9; i++)
        {
            directions.push_back(make_pair(i, 2));
        }
        break;
    }
    default:
    {
        break;
    }
    }
    for (auto &dir : directions)
    {
        int dr = ((dir.first - 1) / 3 - 1) * dir.second;
        int dc = ((dir.first - 1) % 3 - 1) * dir.second;
        if (dr == 0 && dc == 0)
        {
            continue;
        }
        if (r0 + dr >= MapHeight || c0 + dc >= MapWidth || r0 + dr < 0 || c0 + dc < 0)
        {
            continue;
        }
        if (this->getUnit(r0 + dr, c0 + dc) == nullptr || (this->getUnit(r0 + dr, c0 + dc)->getSide() == sd && this->getUnit(r0 + dr, c0 + dc)->getType() != UNDEFINED))
        {
            continue;
        }
        if (ut == ARCHER && this->getTerrain(r0 + dr / dir.second, c0 + dc / dir.second) == MOUNTAIN)
        {
            continue;
        }
        canAttackBlocks.push_back(make_pair(r0 + dr, c0 + dc));
    }
    return canAttackBlocks;
}