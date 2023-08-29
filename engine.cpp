#include <sstream>
#include <iomanip>
#include <cassert>
#include <queue>
#include <memory>
#include <cstring>
#include "engine.h"
#include "units.h"
#include <iomanip>
using namespace std;

void GameLogic(string line, Field *f, bool side)
{
    // ����ϻغϵĻ�
    for (int i = 0; i < f->getWidth(); i++)
    {
        for (int j = 0; j < f->getHeight(); j++)
        {
            if (f->getTerrain(i, j) == ONFIRE)
            {
                f->setTerrain(i, j, PLAIN);
            }
        }
    }
    // �ƶ��͹���
    int r = 0, c = 0, d = 0;
    int final_r = 0, final_c = 0;
    int energy = 0; // �ж���
                    //    string line;
                    //    if(!std::getline(is, line)){
                    //        return;
                    //    }
    vector<int> op;
    for (int i = 0; i < line.length(); i++)
    {
        char ch = line.at(i);
        if (isblank(ch))
        {
            continue;
        }
        if (ch == '1' && i < line.length() - 1 && line.at(i + 1) == '0')
        {
            op.push_back(10);
            i++;
            continue;
        }
        op.push_back(ch - '0');
    }
    if (op.empty())
    {
        return;
    }
    int idx = 0;
    r = op.at(idx);
    idx++;
    c = op.at(idx);
    idx++;
    if (f->getUnit(r, c) == nullptr || f->getUnit(r, c)->getType() == UNDEFINED)
    {
        cout << "There is no chess in this position" << endl;
        return;
    }
    if (side != f->getUnit(r, c)->getSide())
    {
        cout << "You can only move your chess" << endl;
        return;
    }
    final_r = r, final_c = c;
    int dx, dy; // d=123456789��dx = -1, 0, 1 = (d-1)/3-1; dy = (d-1)%3 - 1, x == row, y == col

    UnitType t = f->getUnit(r, c)->getType();
    switch (t)
    {
    case FOOTMAN:
        energy = 4;
        break;
    case KNIGHT:
        energy = 5;
        break;
    case ARCHER:
        energy = 3;
        break;
    case MAGE:
        energy = 2;
        break;
    case UNDEFINED:
        break;
    }
    while (energy > 0)
    {
        if (idx >= op.size())
        {
            break;
        }
        d = op.at(idx);
        if (d == 0 || d > 9)
        {
            break;
        }
        if (d == 5)
        {
            continue;
        }
        idx++;
        dx = (d - 1) / 3 - 1;
        dy = (d - 1) % 3 - 1;
        if (!f->inBounds(r + dx, c + dy))
        {
            break;
        }
        Terrain newplace = f->getTerrain(r + dx, c + dy);
        int cost = 0;
        switch (newplace)
        {
        case PLAIN:
            cost = 1;
            break;
        case FOREST:
            cost = 2;
            break;
        case WATER:
            cost = 999;
            break;
        case MOUNTAIN:
            cost = 999;
            break;
        case ABYSS:
            cost = 999;
            break;
        }
        if (energy >= cost && f->inBounds(r + dx, c + dy))
        {
            int something = 0; // doesn't matter in console version
            f->moveUnit(r, c, r + dx, c + dy, something);
            energy -= cost;
            r += dx;
            c += dy;
        }
    }
    if (idx < op.size())
    {
        d = op.at(idx); // ����energy=0����û����d
    }

    final_r = r;
    final_c = c;
    if (d == 0)
    {
        // �����߼�
        idx++;
        d = op.at(idx); // ����
        dx = (d - 1) / 3 - 1;
        dy = (d - 1) % 3 - 1;
        if (t != ARCHER && t != MAGE)
        {
            // �����ٽ�һ��
            f->getUnit(final_r + dx, final_c + dy)->setType(UNDEFINED);
        }
        else if (f->getTerrain(final_r + dx, final_c + dy) != MOUNTAIN)
        {
            // ARCHER������һ�����û�и�ɽ����Թ���
            f->getUnit(final_r + 2 * dx, final_c + 2 * dy)->setType(UNDEFINED);
        }
        else
        {
            cout << "Illegal input!The units is " << t << "but you input 0" << endl;
        }
    }
    else if (d == 10)
    {
        // ��ʦ, 10 D S
        idx++;
        d = op.at(idx);
        idx++;
        int s = op.at(idx); // s=1����������s=2������
        switch (s)
        {
        case 1:
        {
            // �˴���������ʵ����˲ʱ�ģ�����дUI��ʱ��Ҫ��
            createFireBall(f, final_r, final_c, d);
            break;
        }
        case 2:
        {
            createEarthquake(f, final_r, final_c, d);
        }
        default:
            break;
        }
    }
}
void createFireBall(Field *f, int r, int c, int d)
{
    bool existFireBall = true;
    int dx = (d - 1) / 3 - 1;
    int dy = (d - 1) % 3 - 1;
    int fbx = r + dx, fby = c + dy;
    std::queue<pair<int, int>> q;
    int visited[20][20] = {0};
    while (existFireBall)
    {

        if (!f->terrains.inBounds(fbx, fby) || f->getTerrain(fbx, fby) == MOUNTAIN)
        {
            existFireBall = false;
            break;
        }
        else if (f->getTerrain(fbx, fby) == FOREST)
        {
            // bfs

            q.push(make_pair(fbx, fby));
            // �Լ�������ջ���
            pair<int, int> nearest[9];
            nearest[0] = make_pair(-1, -1);
            nearest[1] = make_pair(0, -1);
            nearest[2] = make_pair(1, -1);
            nearest[3] = make_pair(-1, 0);
            nearest[4] = make_pair(1, 0);
            nearest[5] = make_pair(-1, 1);
            nearest[6] = make_pair(0, 1);
            nearest[7] = make_pair(1, 1);
            nearest[8] = make_pair(0, 0);
            while (!q.empty())
            {
                for (int i = 0; i < 9; i++)
                {
                    if (!f->terrains.inBounds(q.front().first + nearest[i].first, q.front().second + nearest[i].second))
                        continue;
                    pair<int, int> edgePoint = make_pair(q.front().first + nearest[i].first, q.front().second + nearest[i].second);
                    if (visited[edgePoint.first][edgePoint.second] || f->getTerrain(edgePoint.first, edgePoint.second) != FOREST)
                        continue;
                    q.push(edgePoint);
                    // fire
                    visited[edgePoint.first][edgePoint.second] = 1;
                    f->getUnit(edgePoint.first, edgePoint.second)->setType(UNDEFINED);
                    f->onFire(edgePoint.first, edgePoint.second);
                }
                q.pop();
            }
        }
        else if (f->getUnit(fbx, fby)->getType() != UNDEFINED)
        {
            f->getUnit(fbx, fby)->setType(UNDEFINED);
            existFireBall = false;
        }
        fbx += dx;
        fby += dy;
    }
}

void createEarthquake(Field *f, int r, int c, int d)
{
    int dx = (d - 1) / 3 - 1;
    int dy = (d - 1) % 3 - 1;
    if (dx != 0 && dy != 0)
    {
        cout << "Earthquake only have four direction!<" << endl;
        return;
    }
    int tx = r + 3 * dx;
    int ty = c + 3 * dy;
    int mid[5][2] = {{0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    for (int i = 0; i < 5; i++)
    {
        int edge_x = tx + mid[i][0];
        int edge_y = ty + mid[i][1];
        if (!f->inBounds(edge_x, edge_y))
        {
            continue;
        }
        if (f->terrains[edge_x][edge_y] == FOREST || f->terrains[edge_x][edge_y] == PLAIN)
        {
            f->setTerrain(edge_x, edge_y, ABYSS);
            f->getUnit(edge_x, edge_y)->setType(UNDEFINED);
        }
        if (f->terrains[edge_x][edge_y] == MOUNTAIN)
        {
            f->setTerrain(edge_x, edge_y, PLAIN);
        }
    }

    // ��ˮ
    Flow(f);
}
bool isGameOver(Field &f)
{
    bool sd1_units = false, sd2_units = false;
    for (int r = 0; r < f.getWidth(); r++)
    {
        for (int c = 0; c < f.getHeight(); c++)
        {
            Unit *u = f.getUnit(r, c);
            if (u->getType() != UNDEFINED)
            {
                if (u->getSide())
                    sd1_units = true;
                else
                    sd2_units = true;
            }
            if (sd1_units && sd2_units)
            {
                return false;
            }
        }
    }
    return true;
}
int whoWin(Field &f)
{
    // return 1 if player1 win, -1 if player2, 0 if draw
    assert(isGameOver(f));
    for (int r = 0; r < f.getWidth(); r++)
    {
        for (int c = 0; c < f.getHeight(); c++)
        {
            Unit *u = f.getUnit(r, c);
            if (u->getType() != UNDEFINED)
            {
                if (u->getSide())
                    return 1;
                else
                    return -1;
            }
        }
    }
    return 0;
}
// Main loop for playing the game

/* commandline play logic below */

// void play(Field &field, istream &is, ostream &os)
// {
//     bool side = true;
//     int numTurns = 1;
//     while (is)
//     {
//         // Print the new map
//         os << field << endl;

//         string player;
//         if (side)
//             player = "Player A";
//         else
//             player = "Player B";

//         os << "Turn " << numTurns << " (" << player << ")" << endl;

//         string line;
//         while (line.length() == 0)
//         {
//             getline(is, line);
//         }
//         stringstream ss(line);

//         // Fill in your code here
//         GameLogic(line, &field, side);
//         if (isGameOver(field))
//         {
//             int who = whoWin(field);
//             switch (who)
//             {
//             case 1:
//                 cout << "Winner is Player A!" << endl;
//                 return;
//             case -1:
//                 cout << "Winner is Player B!" << endl;
//                 return;
//             case 0:
//                 cout << "A Draw!" << endl;
//                 return;
//             }
//             break;
//         }
//         side = !side;
//         numTurns++;
//     }
// }
void Flow(Field *f)
{
    // ������Ԩ��ˮ�������е���ˮ�� bfs
    int visited[20][20] = {0};

    for (int i = 0; i < f->getWidth(); i++)
    {
        for (int j = 0; j < f->getHeight(); j++)
        {
            if (f->getTerrain(i, j) == WATER)
            {
                std::queue<pair<int, int>> q;
                q.push(make_pair(i, j));
                visited[i][j] = 1;
                pair<int, int> nearest[8];
                nearest[0] = make_pair(-1, -1);
                nearest[1] = make_pair(0, -1);
                nearest[2] = make_pair(1, -1);
                nearest[3] = make_pair(-1, 0);
                nearest[4] = make_pair(1, 0);
                nearest[5] = make_pair(-1, 1);
                nearest[6] = make_pair(0, 1);
                nearest[7] = make_pair(1, 1);
                while (!q.empty())
                {
                    for (int k = 0; k < 8; k++)
                    {
                        if (!f->terrains.inBounds(q.front().first + nearest[k].first, q.front().second + nearest[k].second))
                            continue;
                        pair<int, int> edgePoint = make_pair(q.front().first + nearest[k].first, q.front().second + nearest[k].second);
                        if (f->getTerrain(edgePoint.first, edgePoint.second) != ABYSS)
                            continue;
                        if (visited[edgePoint.first][edgePoint.second])
                            continue;
                        visited[edgePoint.first][edgePoint.second] = 1;
                        q.push(edgePoint);
                        f->setTerrain(edgePoint.first, edgePoint.second, WATER);
                    }
                    q.pop();
                }
            }
        }
    }
}

Field *loadmap(istream &is)
{
    int m, n, nt, mu; // m*n field, nt special places, mu special units
    is >> m >> n >> nt >> mu;
    Field *f = new Field(m, n);
    // ��ʼ��
    for (int i = 0; i < f->getHeight(); i++)
    {
        for (int j = 0; j < f->getWidth(); j++)
        {
            f->units[i][j] = new Unit(UNDEFINED, true);
        }
    }
    // ��ȡ����

    for (int i = 0; i < nt; i++)
    {
        int r, c;
        char t;
        is >> r >> c >> t;
        switch (t)
        {
        case 'W':
            f->terrains[r][c] = WATER;
            break;
        case 'F':
            f->terrains[r][c] = FOREST;
            break;
        case 'M':
            f->terrains[r][c] = MOUNTAIN;
            break;
        case 'A':
            f->terrains[r][c] = ABYSS;
            break;
        default:
            cout << "Failed to load map!" << endl;
        }
    }
    for (int i = 0; i < mu; i++)
    {
        int r, c;
        char s;
        // char* u = nullptr;
        string u;
        is >> r >> c >> s >> u;
        bool sd = (s == 'A');
        if (!f->canOver(r, c))
        {
            f->units[r][c] = nullptr;
            continue;
        }
        if (u == "FT")
        {
            f->units[r][c] = new Unit(FOOTMAN, sd);
        }
        else if (u == "KN")
        {
            f->units[r][c] = new Unit(KNIGHT, sd);
        }
        else if (u == "AR")
        {
            f->units[r][c] = new Unit(ARCHER, sd);
        }
        else if (u == "MG")
        {
            f->units[r][c] = new Unit(MAGE, sd);
        }
        else
        {
            cout << "Failed to load map!" << endl;
        }
    }
    // ��ˮ
    Flow(f);
    // ������Խ
    for (int i = 0; i < f->getHeight(); i++)
    {
        for (int j = 0; j < f->getWidth(); j++)
        {
            if (!f->canOver(i, j))
            {
                delete f->units[i][j];
                f->units[i][j] = new Unit(UNDEFINED, true);
            }
        }
    }

    return f;
}
