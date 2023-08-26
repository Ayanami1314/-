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
using namespace std;
class battleField: public olc::PixelGameEngine{
private:
    // load the resource


    //init
    int MapWidth = 400;
    int MapHeight = 400;
    int MapSize = 20;
    int blockSize = MapWidth / MapSize;
    unique_ptr<int[]> blocks = make_unique<int[]>(MapSize * MapSize);

    unique_ptr<olc::Decal> (*codePicMap)(int) = [](int code){
        // mask code: 0-9 characters, 10-19 terrain, 20-39 user_input, 40-59 attached-effects
        // 1 red_ft, 2 red_kn, 3 red_ar, 4 red_mg
        // -1, -2, -3, -4 blue
        // 10 plain, 11 water, 12 mountains, 13 abyss, 19 fire
        // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack
        std::unique_ptr<olc::Sprite> red_knight = make_unique<olc::Sprite>("./vector-icon/red-knight.png");
        std::unique_ptr<olc::Sprite> blue_knight = make_unique<olc::Sprite>("./vector-icon/blue-knight.png");
        std::unique_ptr<olc::Sprite> red_archer = make_unique<olc::Sprite>("./vector-icon/red-archer.png");
        std::unique_ptr<olc::Sprite> blue_archer = make_unique<olc::Sprite>("./vector-icon/blue-archer.png");
        std::unique_ptr<olc::Sprite> red_wizard = make_unique<olc::Sprite>("./vector-icon/red-wizard.png");
        std::unique_ptr<olc::Sprite> blue_wizard = make_unique<olc::Sprite>("./vector-icon/blue-wizard.png");
        std::unique_ptr<olc::Sprite> red_footman = make_unique<olc::Sprite>("./vector-icon/red-footman.png");
        std::unique_ptr<olc::Sprite> blue_footman = make_unique<olc::Sprite>("./vector-icon/blue-footman.png");
        std::unique_ptr<olc::Sprite> forest = make_unique<olc::Sprite>("./vector-icon/forest.png");
        std::unique_ptr<olc::Sprite> water = make_unique<olc::Sprite>("./vector-icon/water.png");
        std::unique_ptr<olc::Sprite> mountains = make_unique<olc::Sprite>("./vector-icon/mountains.png");
        std::unique_ptr<olc::Sprite> abyss = make_unique<olc::Sprite>("./vector-icon/hole.png");
        std::unique_ptr<olc::Sprite> fire = make_unique<olc::Sprite>("./vector-icon/fire.png");
        std::unique_ptr<olc::Sprite> fireball = make_unique<olc::Sprite>("./vector-icon/fire-zone.png");
        std::unique_ptr<olc::Sprite> earthquake = make_unique<olc::Sprite>("./vector-icon/earth-spit.png");
        std::unique_ptr<olc::Sprite> choice_arrow = make_unique<olc::Sprite>("./vector-icon/crosshair.png");
        std::unique_ptr<olc::Sprite> move_arrow = make_unique<olc::Sprite>("./vector-icon/move.png");
        std::unique_ptr<olc::Sprite> attack = make_unique<olc::Sprite>("./vector-icon/knife-thrust.png");
        std::unique_ptr<olc::Sprite> map = make_unique<olc::Sprite>("./vector-icon/plain-square.png");
        std::unique_ptr<olc::Decal> red_knight_decal = make_unique<olc::Decal>(red_knight.get());
        std::unique_ptr<olc::Decal> blue_knight_decal = make_unique<olc::Decal>(blue_knight.get());
        std::unique_ptr<olc::Decal> red_archer_decal = make_unique<olc::Decal>(red_archer.get());
        std::unique_ptr<olc::Decal> blue_archer_decal = make_unique<olc::Decal>(blue_archer.get());
        std::unique_ptr<olc::Decal> red_wizard_decal = make_unique<olc::Decal>(red_wizard.get());
        std::unique_ptr<olc::Decal> blue_wizard_decal = make_unique<olc::Decal>(blue_wizard.get());
        std::unique_ptr<olc::Decal> map_decal = make_unique<olc::Decal>(map.get());
        std::unique_ptr<olc::Decal> forest_decal = make_unique<olc::Decal>(forest.get());
        std::unique_ptr<olc::Decal> water_decal = make_unique<olc::Decal>(water.get());
        std::unique_ptr<olc::Decal> mountains_decal = make_unique<olc::Decal>(mountains.get());
        std::unique_ptr<olc::Decal> fire_decal = make_unique<olc::Decal>(fire.get());
        std::unique_ptr<olc::Decal> attack_decal = make_unique<olc::Decal>(attack.get());
        std::unique_ptr<olc::Decal> move_arrow_decal = make_unique<olc::Decal>(move_arrow.get());
        std::unique_ptr<olc::Decal> fireball_decal = make_unique<olc::Decal>(fireball.get());
        std::unique_ptr<olc::Decal> earthquake_decal = make_unique<olc::Decal>(earthquake.get());
        std::unique_ptr<olc::Decal> abyss_decal = make_unique<olc::Decal>(abyss.get());
        std::unique_ptr<olc::Decal> choice_arrow_decal = make_unique<olc::Decal>(choice_arrow.get());
        std::unique_ptr<olc::Decal> red_footman_decal = make_unique<olc::Decal>(red_footman.get());
        std::unique_ptr<olc::Decal> blue_footman_decal = make_unique<olc::Decal>(blue_footman.get());
        switch(code){
            // mask code: 0-9 characters, 10-19 terrain, 20-39 user_input, 40-59 attached-effects
            // 1 red_ft, 2 red_kn, 3 red_ar, 4 red_mg
            // -1, -2, -3, -4 blue
            // 10 plain, 11 water, 12 mountains, 13 abyss, 19 fire
            // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack
            case 1: return red_footman_decal;
            case 2: return red_knight_decal;
            case 3: return red_archer_decal;
            case 4: return red_wizard_decal;
            case -1: return blue_footman_decal;
            case -2: return blue_knight_decal;
            case -3: return blue_archer_decal;
            case -4: return blue_wizard_decal;
            case 10: return map_decal;
            case 11: return water_decal;
            case 12: return mountains_decal;
            case 13: return abyss_decal;
            case 19: return fire_decal;
            case 20: return fireball_decal;
            case 21: return earthquake_decal;
            case 22: return move_arrow_decal;
            case 23: return choice_arrow_decal;
            case 24: return attack_decal;
            default: return map_decal;
        }

    };
public:
    battleField(){
        sAppName = "battleField";
    }
    bool OnUserCreate() override{
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override{
        Clear(olc::WHITE);
        SetPixelMode(olc::Pixel::MASK);

        for(int i = 0; i < MapSize * MapSize; i++){
            blocks[i] = 10; // plain
            DrawDecal(olc::vi2d(i/MapSize, i%MapSize), codePicMap(blocks[i]).get(), olc::vf2d(float(blockSize)/512.0, float(blockSize)/512.0));

        }
        return true;
    }


};
int main(){
    battleField bf;
    bf.Construct(1280, 720, 1, 1);
    bf.Start();
    return 0;
}