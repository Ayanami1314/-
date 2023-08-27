//
// Created by Ayanami on 2023/8/23.
//
#define OLC_PGE_APPLICATION
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <vector>
#include "field.h"
#include "units.h"
#include "engine.h"
#include "olcPixelGameEngine.h"
#include "utility.h"
using namespace std;

class battleField : public olc::PixelGameEngine
{
private:
    // load the resource

    // init
    const int MapWidth = 450;
    const int MapHeight = 450;
    const int MapSize = 9;
    const int blockSize = MapWidth / MapSize;
    int turnNumber = 0;
    string whosTurn = "Red";
    unique_ptr<int[]> blocks = make_unique<int[]>(MapSize * MapSize);
    unique_ptr<int[]> mapCache = make_unique<int[]>(MapSize * MapSize);
    vector<int> onClick;
    unique_ptr<olc::Sprite> (*codePicMap)(int) = [](int code)
    {
        // mask code: 0-9 characters, 10-19 terrain, 20-39 user_input, 40-59 attached-effects
        // 1 red_ft, 2 red_kn, 3 red_ar, 4 red_mg
        // -1, -2, -3, -4 blue
        // 10 plain, 11 water, 12 mountains, 13 abyss, 14 forest,  19 fire
        // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack
        std::unique_ptr<olc::Sprite> red_knight = make_unique<olc::Sprite>("./vector_icon_resized/red-knight.png");
        std::unique_ptr<olc::Sprite> blue_knight = make_unique<olc::Sprite>("./vector_icon_resized/blue-knight.png");
        std::unique_ptr<olc::Sprite> red_archer = make_unique<olc::Sprite>("./vector_icon_resized/red-archer.png");
        std::unique_ptr<olc::Sprite> blue_archer = make_unique<olc::Sprite>("./vector_icon_resized/blue-archer.png");
        std::unique_ptr<olc::Sprite> red_wizard = make_unique<olc::Sprite>("./vector_icon_resized/red-wizard.png");
        std::unique_ptr<olc::Sprite> blue_wizard = make_unique<olc::Sprite>("./vector_icon_resized/blue-wizard.png");
        std::unique_ptr<olc::Sprite> red_footman = make_unique<olc::Sprite>("./vector_icon_resized/red-footman.png");
        std::unique_ptr<olc::Sprite> blue_footman = make_unique<olc::Sprite>("./vector_icon_resized/blue-footman.png");
        std::unique_ptr<olc::Sprite> forest = make_unique<olc::Sprite>("./vector_icon_resized/forest.png");
        std::unique_ptr<olc::Sprite> water = make_unique<olc::Sprite>("./vector_icon_resized/water.png");
        std::unique_ptr<olc::Sprite> mountains = make_unique<olc::Sprite>("./vector_icon_resized/mountains.png");
        std::unique_ptr<olc::Sprite> abyss = make_unique<olc::Sprite>("./vector_icon_resized/hole.png");
        std::unique_ptr<olc::Sprite> fire = make_unique<olc::Sprite>("./vector_icon_resized/fire.png");
        std::unique_ptr<olc::Sprite> fireball = make_unique<olc::Sprite>("./vector_icon_resized/fire-zone.png");
        std::unique_ptr<olc::Sprite> earthquake = make_unique<olc::Sprite>("./vector_icon_resized/earth-spit.png");
        std::unique_ptr<olc::Sprite> choice_arrow = make_unique<olc::Sprite>("./vector_icon_resized/crosshair.png");
        std::unique_ptr<olc::Sprite> move_arrow = make_unique<olc::Sprite>("./vector_icon_resized/move.png");
        std::unique_ptr<olc::Sprite> attack = make_unique<olc::Sprite>("./vector_icon_resized/knife-thrust.png");
        std::unique_ptr<olc::Sprite> map = make_unique<olc::Sprite>("./vector_icon_resized/plain-square.png");
        //        std::unique_ptr<olc::Decal> red_knight_decal = make_unique<olc::Decal>(red_knight.get());
        //        std::unique_ptr<olc::Decal> blue_knight_decal = make_unique<olc::Decal>(blue_knight.get());
        //        std::unique_ptr<olc::Decal> red_archer_decal = make_unique<olc::Decal>(red_archer.get());
        //        std::unique_ptr<olc::Decal> blue_archer_decal = make_unique<olc::Decal>(blue_archer.get());
        //        std::unique_ptr<olc::Decal> red_wizard_decal = make_unique<olc::Decal>(red_wizard.get());
        //        std::unique_ptr<olc::Decal> blue_wizard_decal = make_unique<olc::Decal>(blue_wizard.get());
        //        std::unique_ptr<olc::Decal> map_decal = make_unique<olc::Decal>(map.get());
        //        std::unique_ptr<olc::Decal> forest_decal = make_unique<olc::Decal>(forest.get());
        //        std::unique_ptr<olc::Decal> water_decal = make_unique<olc::Decal>(water.get());
        //        std::unique_ptr<olc::Decal> mountains_decal = make_unique<olc::Decal>(mountains.get());
        //        std::unique_ptr<olc::Decal> fire_decal = make_unique<olc::Decal>(fire.get());
        //        std::unique_ptr<olc::Decal> attack_decal = make_unique<olc::Decal>(attack.get());
        //        std::unique_ptr<olc::Decal> move_arrow_decal = make_unique<olc::Decal>(move_arrow.get());
        //        std::unique_ptr<olc::Decal> fireball_decal = make_unique<olc::Decal>(fireball.get());
        //        std::unique_ptr<olc::Decal> earthquake_decal = make_unique<olc::Decal>(earthquake.get());
        //        std::unique_ptr<olc::Decal> abyss_decal = make_unique<olc::Decal>(abyss.get());
        //        std::unique_ptr<olc::Decal> choice_arrow_decal = make_unique<olc::Decal>(choice_arrow.get());
        //        std::unique_ptr<olc::Decal> red_footman_decal = make_unique<olc::Decal>(red_footman.get());
        //        std::unique_ptr<olc::Decal> blue_footman_decal = make_unique<olc::Decal>(blue_footman.get());
        switch (code)
        {
        // mask code: 0-9 characters, 10-19 terrain, 20-39 user_input, 40-59 attached-effects
        // 1 red_ft, 2 red_kn, 3 red_ar, 4 red_mg
        // -1, -2, -3, -4 blue
        // 10 plain, 11 water, 12 mountains, 13 abyss, 19 fire
        // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack
        case 1:
            return red_footman;
        case 2:
            return red_knight;
        case 3:
            return red_archer;
        case 4:
            return red_wizard;
        case -1:
            return blue_footman;
        case -2:
            return blue_knight;
        case -3:
            return blue_archer;
        case -4:
            return blue_wizard;
        case 10:
            return map;
        case 11:
            return water;
        case 12:
            return mountains;
        case 13:
            return abyss;
        case 14:
            return forest;
        case 19:
            return fire;
        case 20:
            return fireball;
        case 21:
            return earthquake;
        case 22:
            return move_arrow;
        case 23:
            return choice_arrow;
        case 24:
            return attack;
        default:
            return map;
        }
    };

public:
    // need to complete
    battleField()
    {
        sAppName = "battleField";
    }
    bool OnUserCreate() override
    {
        loadSmallMap(blocks);
        loadEmptyMap(mapCache, MapSize);
        SetPixelMode(olc::Pixel::MASK);
        Clear(olc::BLACK);
        onClick.resize(MapSize * MapSize);
        onClick.assign(MapSize * MapSize, 0);
        return true;
    }
    bool whichBlockClicked(int x, int y, int upReserved = 2)
    {
        // upReserved: Title area
        if (x < 0 || y < upReserved * blockSize)
            return false; // click invalid area
        for (int i = 0; i < MapSize * MapSize; ++i)
        {
            onClick[i] = 0; // clear other state
        }
        onClick[x / blockSize + (y / blockSize - upReserved) * MapSize] = 1;
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override
    {
        // get mouse click information
        bool clicked = false;
        if (GetMouse(0).bPressed)
        {
            int clickX = GetMouseX();
            int clickY = GetMouseY();
            clicked = whichBlockClicked(clickX, clickY);
        }
        // upper 2 block size: (who's turn, turn number)
        DrawString(0, 0, "Turns: " + turnNumber, olc::WHITE, 3u);
        DrawString(0, blockSize, "Player " + whosTurn, olc::WHITE, 3u);
        // sideBar

        if (clicked || isMapChanged(mapCache.get(), blocks.get(), MapSize * MapSize))
        {
            // only when map changed or clicked valid area rerender map
            Clear(olc::BLACK);
            for (int i = 0; i < MapSize * MapSize; i++)
            {
                // + 2, leave some space for title
                // x: col, y: row, in map index, (r, c) -> (y, x)
                // i: row*MapSize + col, x = i % MapSize, y = 2 + i / MapSize
                DrawSprite(olc::vi2d(i % MapSize, i / MapSize + 2) * blockSize, codePicMap(blocks[i]).get());
                if (onClick[i] != 0)
                {
                    // choose picture
                    DrawSprite(olc::vi2d(i % MapSize, i / MapSize + 2) * blockSize, codePicMap(23).get());
                }
            }
            arrayCopy(blocks.get(), mapCache.get(), MapSize * MapSize);
        }

        return true;
    }
};
int main()
{
    battleField bf;
    bf.Construct(800, 800, 2, 2);
    bf.Start();
    return 0;
}