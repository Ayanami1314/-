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
class Area
{
public:
    int posX;
    int posY;
    int width;
    int height;
    bool in(int x, int y)
    {
        return x >= this->posX && x <= this->posX + this->width && y >= this->posY && y <= this->posY + this->height;
    }
    Area()
    {
        posX = 0;
        posY = 0;
        width = 0;
        height = 0;
    };
    Area(const Area &a) = default;
    ~Area() = default;
    Area(int x, int y, int w, int h)
    {
        this->posX = x;
        this->posY = y;
        this->width = w;
        this->height = h;
    }
    olc::vi2d getLeftTop()
    {
        return olc::vi2d(this->posX, this->posY);
    }
    olc::vi2d getRightBottom()
    {
        return olc::vi2d(this->posX + this->width, this->posY + this->height);
    }
};
class clickButton : public Area
{
public:
    bool clicked = false;
    bool exist = false;

    clickButton(int x, int y, int w, int h, bool c = false) : Area(x, y, w, h)
    {
        this->clicked = c;
    }
    clickButton() = default;
    clickButton(const clickButton &c) = default;
    ~clickButton() = default;
    void printInfo()
    {
        cout << "clicked: " << clicked << ", exist: " << exist << endl;
    }
    // void setButtonLoc(int x, int y, int w, int h)
    // {
    //     this->posX = x;
    //     this->posY = y;
    //     this->width = w;
    //     this->height = h;
    // }
};
struct sideBar
{
    int posX;
    int posY;
    int width;
    int height;
};
const int MapWidth = 450;
const int MapHeight = 450;

const int blockSize = 50;
const int MapSize = MapWidth / blockSize; // 9
Field *f = new Field(MapSize, MapSize);
int sideBarLeftTopX = MapSize * blockSize * 32 / 30;
int sideBarLeftTopY = blockSize;

clickButton *pMoveButton = new clickButton(sideBarLeftTopX, sideBarLeftTopY + 2 * blockSize, blockSize, blockSize);
clickButton *pAttackButton = new clickButton(sideBarLeftTopX + blockSize, sideBarLeftTopY + 2 * blockSize, blockSize, blockSize);
clickButton *pFireBall = new clickButton(sideBarLeftTopX, sideBarLeftTopY + 4 * blockSize, blockSize, blockSize);
clickButton *pEarthquake = new clickButton(sideBarLeftTopX + blockSize, sideBarLeftTopY + 4 * blockSize, blockSize, blockSize);
clickButton *pCancelButton = new clickButton(sideBarLeftTopX, sideBarLeftTopY + 6 * blockSize, 2 * blockSize, blockSize);
clickButton *pEndTurnButton = new clickButton(sideBarLeftTopX, sideBarLeftTopY + 8 * blockSize, blockSize, blockSize);
Area *pMoveStepArea = new Area(sideBarLeftTopX, sideBarLeftTopY + blockSize, 2 * blockSize, blockSize);
Area *hintArea = new Area(sideBarLeftTopX, sideBarLeftTopY + 10 * blockSize, 2 * blockSize, blockSize);
class battleField : public olc::PixelGameEngine
{
private:
    // load the resource

    // init

    int turnNumber = 0;
    string whosTurn = "Red";
    // terrain:first layer, character: second layer
    unique_ptr<pair<int, int>[]> blocks = make_unique<pair<int, int>[]>(MapSize * MapSize);
    unique_ptr<pair<int, int>[]> mapCache = make_unique<pair<int, int>[]>(MapSize * MapSize);
    vector<int> onClick; // blocks on the map
    // int sideBarLeftTopX = MapSize * blockSize * 32 / 30;
    // int sideBarLeftTopY = blockSize;
    // clickButton moveButton = *pMoveButton;
    // clickButton attackButton = *pAttackButton;
    // clickButton fireBall = *pFireBall;
    // clickButton earthquake = *pEarthquake;
    // clickButton cancelButton = *pCancelButton;
    sideBar sbar;
    Area moveStepArea = *pMoveStepArea;
    int lastChooseBlockIndex = -1;
    int remainSteps = 0;
    unique_ptr<olc::Sprite> (*codePicMap)(int) = [](int code)
    {
        // mask code: 0-9 characters, 10-19 terrain, 20-39 user_input, 40-59 attached-effects
        // 1 red_ft, 2 red_kn, 3 red_ar, 4 red_mg
        // -1, -2, -3, -4 blue
        // 10 plain, 11 water, 12 mountains, 13 abyss, 14 forest,  19 fire
        // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack, 25 cancel, 26 player_next, 27 button_click

        std::unique_ptr<olc::Sprite> red_knight = make_unique<olc::Sprite>("./vector_icon_resized/red-knight.png");
        std::unique_ptr<olc::Sprite> blue_knight = make_unique<olc::Sprite>("./vector_icon_resized/blue-knight.png");
        std::unique_ptr<olc::Sprite> red_archer = make_unique<olc::Sprite>("./vector_icon_resized/red-archer.png");
        std::unique_ptr<olc::Sprite> blue_archer = make_unique<olc::Sprite>("./vector_icon_resized/blue-archer.png");
        std::unique_ptr<olc::Sprite> red_wizard = make_unique<olc::Sprite>("./vector_icon_resized/red-wage.png");
        std::unique_ptr<olc::Sprite> blue_wizard = make_unique<olc::Sprite>("./vector_icon_resized/blue-wage.png");
        std::unique_ptr<olc::Sprite> red_footman = make_unique<olc::Sprite>("./vector_icon_resized/red-footman.png");
        std::unique_ptr<olc::Sprite> blue_footman = make_unique<olc::Sprite>("./vector_icon_resized/blue-footman.png");
        std::unique_ptr<olc::Sprite> forest = make_unique<olc::Sprite>("./vector_icon_resized/forest.png");
        std::unique_ptr<olc::Sprite> water = make_unique<olc::Sprite>("./vector_icon_resized/water.png");
        std::unique_ptr<olc::Sprite> mountains = make_unique<olc::Sprite>("./vector_icon_resized/mountains.png");
        std::unique_ptr<olc::Sprite> abyss = make_unique<olc::Sprite>("./vector_icon_resized/hole.png");
        std::unique_ptr<olc::Sprite> fire = make_unique<olc::Sprite>("./vector_icon_resized/fire.png");
        std::unique_ptr<olc::Sprite> fireball = make_unique<olc::Sprite>("./vector_icon_resized/fire-zone.png");
        std::unique_ptr<olc::Sprite> earthquake = make_unique<olc::Sprite>("./vector_icon_resized/earth-spit.png");
        std::unique_ptr<olc::Sprite> choice_arrow = make_unique<olc::Sprite>("./vector_icon_resized_alpha/crosshair.png");
        std::unique_ptr<olc::Sprite> move_arrow = make_unique<olc::Sprite>("./vector_icon_resized/move.png");
        std::unique_ptr<olc::Sprite> attack = make_unique<olc::Sprite>("./vector_icon_resized/knife-thrust.png");
        std::unique_ptr<olc::Sprite> map = make_unique<olc::Sprite>("./vector_icon_resized/plain-square.png");
        std::unique_ptr<olc::Sprite> cancel = make_unique<olc::Sprite>("./vector_icon_resized/cancel2.png");
        std::unique_ptr<olc::Sprite> player_next = make_unique<olc::Sprite>("./vector_icon_resized/player-next.png");
        std::unique_ptr<olc::Sprite> button_clicked = make_unique<olc::Sprite>("./vector_icon_resized/click.png");
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
        // 10 plain, 11 water, 12 mountains, 14 forest, 13 abyss, 19 fire
        // 20 fireball, 21 earthquake, 22 move_arrow, 23 choice_arrow, 24 attack 25 cancel, 26 player_next, 27 button_clicked
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
        case 25:
            return cancel;
        case 26:
            return player_next;
        case 27:
            return button_clicked;
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
        // loadSmallTestMap(blocks);
        //
        // loadEmptyRedWinMap(blocks, MapSize);
        loadSmallMap(blocks);
        loadEmptyMap(mapCache, MapSize);
        f->loadmap_array(blocks.get(), MapSize);
        Clear(olc::BLACK);
        onClick.resize(MapSize * MapSize);
        onClick.assign(MapSize * MapSize, 0);
        int sideBarLeftTopX = MapSize * blockSize * 32 / 30;
        int sideBarLeftTopY = blockSize;
        // setButtonLoc(sideBarLeftTopX, sideBarLeftTopY + 2 * blockSize, blockSize, blockSize);
        // attackButton.setButtonLoc(sideBarLeftTopX + blockSize, sideBarLeftTopY + 2 * blockSize, blockSize, blockSize);
        // fireBall.setButtonLoc(sideBarLeftTopX, sideBarLeftTopY + 4 * blockSize, blockSize, blockSize);
        // earthquake.setButtonLoc(sideBarLeftTopX + blockSize, sideBarLeftTopY + 4 * blockSize, blockSize, blockSize);
        // cancelButton.setButtonLoc(sideBarLeftTopX, sideBarLeftTopY + 6 * blockSize, 2 * blockSize, blockSize);
        cout << "move area" << pMoveButton->getLeftTop() << pMoveButton->getRightBottom() << endl;
        sbar.posX = sideBarLeftTopX;
        sbar.posY = sideBarLeftTopY;
        sbar.height = MapSize * blockSize;
        sbar.width = MapSize * blockSize / 3;
        moveStepArea = *pMoveStepArea;
        CreateLayer(); // create second layer
        return true;
    }
    void clearButtonState()
    {
        pMoveButton->exist = false;
        pAttackButton->exist = false;
        pFireBall->exist = false;
        pEarthquake->exist = false;
        pCancelButton->exist = false;
        pMoveButton->clicked = false;
        pAttackButton->clicked = false;
        pFireBall->clicked = false;
        pEarthquake->clicked = false;
        pCancelButton->clicked = false;
        pEndTurnButton->clicked = false;
    }
    bool hasButtonClicked()
    {
        return pMoveButton->clicked || pAttackButton->clicked || pFireBall->clicked || pEarthquake->clicked || pCancelButton->clicked || pEndTurnButton->clicked;
    }
    void drawDetectBox(clickButton b)
    {
        if (!b.exist)
            return;
        olc::vi2d v = {b.posX, b.posY};
        olc::vi2d v2 = {b.posX, b.posY + b.height};
        olc::vi2d v3 = {b.posX + b.width, b.posY + b.height};
        olc::vi2d v4 = {b.posX + b.width, b.posY};
        DrawLine(v, v2, olc::GREEN);
        DrawLine(v2, v3, olc::GREEN);
        DrawLine(v3, v4, olc::GREEN);
        DrawLine(v4, v, olc::GREEN);
    }
    bool whichBlockClicked(int x, int y, int upReserved = 2)
    {
        // upReserved: Title area
        // check if and which block be clicked
        for (int i = 0; i < MapSize * MapSize; ++i)
        {
            onClick[i] = 0; // clear state
        }
        bool mapClicked = false;
        if (x < 0 || y < 0)
            return false; // click invalid area
        if (x < MapSize * blockSize && y < MapSize * blockSize + upReserved * blockSize)
        {
            int tar = x / blockSize + (y / blockSize - upReserved) * MapSize;
            onClick[tar] = (blocks[tar].second != 0 ? blocks[tar].second : blocks[tar].first);
            return true;
        }

        // additional effect, judge if click the buttons
        auto checkButton = [x, y](clickButton *pbutton)
        {
            if (pbutton->exist && pbutton->in(x, y))
            {
                pbutton->clicked = true;
                return true;
            }
            return false;
        };
        return checkButton(pCancelButton) || checkButton(pAttackButton) || checkButton(pMoveButton) || checkButton(pEarthquake) || checkButton(pFireBall) || checkButton(pEndTurnButton);
    }
    bool OnUserUpdate(float fElapsedTime) override
    {

        SetPixelMode(olc::Pixel::ALPHA);
        // get mouse click information
        bool clicked = false;

        int characterX = 0;
        int characterY = 0;
        if (GetMouse(0).bPressed)
        {
            int clickX = GetMouseX();
            int clickY = GetMouseY();
            DrawCircle(clickX, clickY, 5, olc::RED);
            // cout << "click pos:" << clickX << "," << clickY << endl;
            // cout << "move area" << pMoveButton->getLeftTop() << pMoveButton->getRightBottom() << endl;
            // cout << "attack in:" << pAttackButton->in(clickX, clickY) << endl;
            // cout << "move in:" << pMoveButton->in(clickX, clickY) << endl;
            clicked = whichBlockClicked(clickX, clickY);
        }
        // upper 2 block size: (who's turn, turn number)
        DrawString(0, 0, "Turns: " + to_string(turnNumber), olc::WHITE, 3u);
        DrawString(0, blockSize, "Player " + whosTurn, olc::WHITE, 3u);
        // right sideBar left & right line width: 1/30 map width, up & bottom line width: half blocksize
        // FillRect(MapSize * blockSize, 0, MapSize * blockSize / 3, MapSize * blockSize, olc::WHITE);
        // FillRect(MapSize * blockSize * 31 / 30, blockSize / 2, MapSize * blockSize * 14 / 45, MapSize * blockSize - blockSize, olc::BLACK);
        int chooseBlockIndex = -1;
        if (clicked || isMapChanged(mapCache.get(), blocks.get(), MapSize * MapSize))
        {
            // only when map changed or clicked valid area rerender map
            Clear(olc::BLACK);
            if (isGameOver(*f))
            {
                DrawString(ScreenWidth() / 4, ScreenHeight() * 1 / 4, "Game Over!", olc::WHITE, 5u);
                string winner = whoWin(*f) == 1 ? "Red Player" : "Blue Player";
                DrawString(ScreenWidth() / 4, ScreenHeight() * 1 / 2, winner + " wins!", olc::WHITE, 5u);
                return true;
            }
            if (clicked)
            {
                cout << "onClick remainSteps:" << remainSteps << endl;
                if (pCancelButton->clicked)
                {
                    // cancel all states
                    clearButtonState();
                    for (int i = 0; i < MapSize * MapSize; ++i)
                    {
                        onClick[i] = 0;
                    }
                    lastChooseBlockIndex = -1;
                    chooseBlockIndex = -1;
                    pCancelButton->exist = false;
                }
                int no = 0;
                for (int i = 0; i < MapSize * MapSize; ++i)
                {
                    if (onClick[i] != 0)
                    {
                        no = onClick[i];
                        chooseBlockIndex = i;
                        break;
                    }
                }
                if (pEndTurnButton->clicked)
                {
                    clearButtonState();
                    if (whosTurn == "Red")
                    {
                        whosTurn = "Blue";
                        pEndTurnButton->clicked = false;
                    }
                    else
                    {
                        whosTurn = "Red";
                        turnNumber++;
                        f->refreshTheState();
                        f->burnOut();
                        pEndTurnButton->clicked = false;
                    }
                }
                // render
                DrawString(sbar.posX, sbar.posY, description(no), olc::WHITE, 2u);

                characterX = chooseBlockIndex % MapSize;
                characterY = chooseBlockIndex / MapSize;
                if (lastChooseBlockIndex == -1)
                    remainSteps = chooseBlockIndex == -1 ? 0 : f->getUnit(characterY, characterX)->getMovePoints();
                else
                {
                    remainSteps = chooseBlockIndex == -1 ? f->getUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize)->getMovePoints() : f->getUnit(characterY, characterX)->getMovePoints();
                }
                // debug information
                // if (lastChooseBlockIndex != -1 && f->getUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize)->getType() == MAGE)
                // {
                //     cout << "MagicNumber: " << f->getUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize)->getMagicNumber() << endl;
                // }
                // debug information
                // cout << "side:" << endl;
                // for (int k = 0; k < MapSize; k++)
                // {
                //     for (int p = 0; p < MapSize; ++p)
                //     {

                //         cout << f->getUnit(k, p)->getSide() << " ";
                //     }
                //     cout << endl;
                // }
                // cout << "f: units" << endl;
                // Unit *u = new Unit(KNIGHT, true);
                // f->setUnit(5, 5, u);
                // for (int k = 0; k < MapSize; k++)
                // {
                //     for (int p = 0; p < MapSize; ++p)
                //     {

                //         cout << f->getUnit(k, p)->getType() << " ";
                //     }
                //     cout << endl;
                // }
                // cout << "f: terrains" << endl;
                // for (int k = 0; k < MapSize; k++)
                // {
                //     for (int p = 0; p < MapSize; ++p)
                //     {
                //         cout << f->getTerrain(k, p) << " ";
                //     }
                //     cout << endl;
                // }
                // cout << "blocks:" << endl;
                // for (int k = 0; k < MapSize; k++)
                // {
                //     for (int p = 0; p < MapSize; ++p)
                //     {
                //         cout << blocks[k * MapSize + p].second << " ";
                //     }
                //     cout << endl;
                // }
                pCancelButton->exist = true;
                pEndTurnButton->exist = true;
                if (no == 4 || no == -4)
                {
                    // mage have two additional choices

                    pMoveButton->exist = true;
                    pFireBall->exist = true;
                    pEarthquake->exist = true;
                    pAttackButton->exist = false;
                }
                else if (abs(no) > 0 && abs(no) < 10) // no=0: click button or other else instead of map blocks
                {
                    pMoveButton->exist = true;
                    pAttackButton->exist = true;
                    pFireBall->exist = false;
                    pEarthquake->exist = false;
                }

                // move & attack logic
                // first click attack or fireball or earthquake, then click block 2
                if (lastChooseBlockIndex != -1 && chooseBlockIndex != -1 && lastChooseBlockIndex != chooseBlockIndex)
                {
                    characterX = lastChooseBlockIndex % MapSize;
                    characterY = lastChooseBlockIndex / MapSize;
                    if (pMoveButton->clicked)
                    {
                        bool side = f->getUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize)->getSide();
                        if (side && whosTurn == "Red" || !side && whosTurn == "Blue")
                        {
                            // update remainSteps
                            remainSteps = f->getUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize)->getMovePoints();
                            cout << "before move: point = " << remainSteps << endl;
                            f->moveUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize, chooseBlockIndex / MapSize, chooseBlockIndex % MapSize, remainSteps);
                            cout << "after move: point = " << remainSteps << endl;
                        }
                        pMoveButton->clicked = false;
                    }
                    else if (pAttackButton->clicked)
                    {
                        // attack consume all the rest of movePoint
                        bool side = f->getUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize)->getSide();
                        if (side && whosTurn == "Red" || !side && whosTurn == "Blue")
                        {
                            cout << "before attack: point = " << remainSteps << endl;
                            f->attack(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize, chooseBlockIndex / MapSize, chooseBlockIndex % MapSize, remainSteps);
                            cout << "after attack: point = " << remainSteps << endl;
                        }
                        pAttackButton->clicked = false;
                    }
                    else if (pFireBall->clicked)
                    {
                        // fireball
                        int direction = getFuzzyFourdirection(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize, chooseBlockIndex / MapSize, chooseBlockIndex % MapSize);
                        int magicNumber = f->getUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize)->getMagicNumber();
                        if (direction != 5 && magicNumber > 0)
                        {
                            createFireBall(f, lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize, direction);
                            pFireBall->clicked = false;
                            f->setUnitMagicNumber(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize, --magicNumber);
                        }
                        DrawString(hintArea->posX, hintArea->posY, "MagicNumber:" + to_string(magicNumber), olc::WHITE, 2u);
                    }
                    else if (pEarthquake->clicked)
                    {
                        // earthquake
                        int direction = getFuzzyFourdirection(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize, chooseBlockIndex / MapSize, chooseBlockIndex % MapSize);
                        int magicNumber = f->getUnit(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize)->getMagicNumber();
                        if (direction != 5 && magicNumber > 0)
                        {
                            createEarthquake(f, lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize, direction);
                            pEarthquake->clicked = false;
                            f->setUnitMagicNumber(lastChooseBlockIndex / MapSize, lastChooseBlockIndex % MapSize, --magicNumber);
                        }
                        DrawString(hintArea->posX, hintArea->posY, "MagicNumber:" + to_string(magicNumber), olc::WHITE, 2u);
                    }
                    f->updateArray(blocks.get(), MapSize);
                }

                if (chooseBlockIndex != -1)
                    lastChooseBlockIndex = chooseBlockIndex;

                // debug information
                cout << "cancelButton:" << endl;
                pCancelButton->printInfo();
                cout << "moveButton:" << endl;
                pMoveButton->printInfo();
                cout << "attackButton:" << endl;
                pAttackButton->printInfo();
                cout << "fireBall:" << endl;
                pFireBall->printInfo();
                cout << "earthquake:" << endl;
                pEarthquake->printInfo();
                cout << "endTurnButton:" << endl;
                pEndTurnButton->printInfo();
                drawDetectBox(*pCancelButton);
                drawDetectBox(*pMoveButton);
                drawDetectBox(*pAttackButton);
                drawDetectBox(*pFireBall);
                drawDetectBox(*pEarthquake);
                drawDetectBox(*pEndTurnButton);

                drawDetectBox(*pFireBall);
                drawDetectBox(*pEarthquake);
                cout << "fireBall:" << endl;
                pFireBall->printInfo();
                cout << "earthquake:" << endl;
                pEarthquake->printInfo();

                // clear onClick
                for (int i = 0; i < MapSize * MapSize; ++i)
                {
                    onClick[i] = 0;
                }
            }
            // render map
            for (int i = 0; i < MapSize * MapSize; i++)
            {
                // + 2, leave some space for title
                // x: col, y: row, in map index, (r, c) -> (y, x)
                // i: row*MapSize + col, x = i % MapSize, y = 2 + i / MapSize

                // layer 1:
                DrawSprite(olc::vi2d(i % MapSize, i / MapSize + 2) * blockSize, codePicMap(blocks[i].first).get());
                if (blocks[i].second != 0)
                {
                    DrawSprite(olc::vi2d(i % MapSize, i / MapSize + 2) * blockSize, codePicMap(blocks[i].second).get());
                }
            }
            if (lastChooseBlockIndex != -1)
                DrawSprite(olc::vi2d(lastChooseBlockIndex % MapSize, lastChooseBlockIndex / MapSize + 2) * blockSize, codePicMap(23).get());
            // layer 2(if exist)
            int renderChooseIndex = chooseBlockIndex != -1 ? chooseBlockIndex : lastChooseBlockIndex;
            int render_X = renderChooseIndex % MapSize;
            int render_Y = renderChooseIndex / MapSize;
            bool flag = (chooseBlockIndex != -1 && isCharacter(blocks[chooseBlockIndex].second)) || (lastChooseBlockIndex != -1 && isCharacter(blocks[lastChooseBlockIndex].second));
            if (flag)
            {
                bool sd = f->getUnit(render_Y, render_X)->getSide();
                if (sd && whosTurn != "Red" || !sd && whosTurn != "Blue")
                {
                    flag = false;
                }
            }
            if (flag && (chooseBlockIndex != -1 || lastChooseBlockIndex != -1))
            {
                // choose picture
                // draw in picture 1

                // row: Y, col:X
                int MovePoints = remainSteps;
                std::cout << f->getUnit(render_Y, render_X)->getType() << endl;
                // int start_i = max(0, render_X - MovePoints);
                // int start_j = max(0, render_Y - MovePoints);
                // int end_i = min(MapSize - 1, render_X + MovePoints);
                // int end_j = min(MapSize - 1, render_Y + MovePoints);
                // SetPixelMode(olc::Pixel::ALPHA);
                // SetLayerTint(1, olc::Pixel(0, 0, 0, 128)); // clear layer 1
                // secondLayer is layer 1, first is 0;
                // SetDrawTarget(1); // draw to layer 1
                cout << "render center:(row, col) = " << olc::vi2d(render_Y, render_X) << endl;
                cout << "remainSteps(in render) = " << remainSteps << endl;
                vector<pair<int, int>> renderBlocks_Move = f->getCanMoveBlocks(render_Y, render_X, remainSteps); // (r, c)
                vector<pair<int, int>> renderBlocks_Attack = f->getCanAttackBlocks(render_Y, render_X);          // (r, c)
                vector<pair<int, int>> renderBlocks_Fireball = f->getFireBallBlocks(render_Y, render_X);         // (r, c)
                vector<pair<int, int>> renderBlocks_Earthquake = f->getEarthquakeBlocks(render_Y, render_X);     // (r, c)
                if (!pAttackButton->clicked)
                {
                    // normal cases it should be rendered
                    olc::Pixel p_alpha_yellow = olc::YELLOW;
                    p_alpha_yellow.a = 128;
                    for (auto &rb : renderBlocks_Move)
                    {
                        cout << "render! (" << rb.first << "," << rb.second << " )" << endl;
                        FillRect(rb.second * blockSize, (rb.first + 2) * blockSize, blockSize, blockSize, p_alpha_yellow);
                    }
                }
                if (pAttackButton->clicked)
                {
                    olc::Pixel p_alpha_red = olc::RED;
                    p_alpha_red.a = 128;
                    for (auto &rb : renderBlocks_Attack)
                    {
                        cout << "render! (" << rb.first << "," << rb.second << " )" << endl; // (r, c)
                        // FillRect(x, y)  (x, y) == (c, r)
                        FillRect(rb.second * blockSize, (rb.first + 2) * blockSize, blockSize, blockSize, p_alpha_red);
                    }
                }
                if (pFireBall->clicked)
                {
                    olc::Pixel p_alpha_red = olc::RED;
                    p_alpha_red.a = 128;
                    for (auto &rb : renderBlocks_Fireball)
                    {
                        cout << "render! (" << rb.first << "," << rb.second << " )" << endl; // (r, c)
                        // FillRect(x, y)  (x, y) == (c, r)
                        FillRect(rb.second * blockSize, (rb.first + 2) * blockSize, blockSize, blockSize, p_alpha_red);
                    }
                }
                if (pEarthquake->clicked)
                {
                    olc::Pixel p_alpha_red = olc::RED;
                    p_alpha_red.a = 128;
                    for (auto &rb : renderBlocks_Earthquake)
                    {
                        cout << "render! (" << rb.first << "," << rb.second << " )" << endl; // (r, c)
                        // FillRect(x, y)  (x, y) == (c, r)
                        FillRect(rb.second * blockSize, (rb.first + 2) * blockSize, blockSize, blockSize, p_alpha_red);
                    }
                }
                // SetDrawTarget(0, true); // draw to layer 0, return
                // EnableLayer(1, true);
            }
            // render sidebar
            auto renderSideBarChoose = [&](clickButton *pb)
            {
                if (pb->clicked)
                {
                    SetDrawTarget(1, true);
                    olc::Pixel choosePixel = olc::GREEN;
                    choosePixel.a = 128;
                    FillRect(pb->posX, pb->posY, pb->width, pb->height, choosePixel);
                    SetDrawTarget(0, false);
                    EnableLayer(1, true);
                }
                return;
            };
            renderSideBarChoose(pMoveButton);
            renderSideBarChoose(pAttackButton);
            renderSideBarChoose(pFireBall);
            renderSideBarChoose(pEarthquake);
            DrawString(moveStepArea.posX, moveStepArea.posY, "MovePoints:" + to_string(remainSteps), olc::WHITE, 2u);
            DrawString(moveStepArea.posX, moveStepArea.posY + 10 * blockSize, "LastChoose:(i,j)=" + to_string(lastChooseBlockIndex / MapSize) + "," + to_string(lastChooseBlockIndex % MapSize), olc::WHITE, 2u);
            DrawString(moveStepArea.posX, moveStepArea.posY + 11 * blockSize, "currentChoose:(i,j)=" + to_string(chooseBlockIndex / MapSize) + "," + to_string(chooseBlockIndex % MapSize), olc::WHITE, 2u);
            if (pFireBall->exist)
                DrawSprite(pFireBall->posX, pFireBall->posY, codePicMap(20).get());
            if (pEarthquake->exist)
                DrawSprite(pEarthquake->posX, pEarthquake->posY, codePicMap(21).get());
            if (pMoveButton->exist)
                DrawSprite(pMoveButton->posX, pMoveButton->posY, codePicMap(22).get()); // move
            if (pAttackButton->exist)
                DrawSprite(pAttackButton->posX, pAttackButton->posY, codePicMap(24).get()); // attack
            DrawSprite(pCancelButton->posX, pCancelButton->posY, codePicMap(25).get());
            DrawSprite(pEndTurnButton->posX, pEndTurnButton->posY, codePicMap(26).get());
            arrayCopy(blocks.get(), mapCache.get(), MapSize * MapSize);
            // f->loadmap_array(blocks.get(), MapSize);  It should be array load field, instead of field load array
        }

        return true;
    }
};
int main()
{
    battleField bf;
    bf.Construct(1000, 800, 2, 2);
    bf.Start();
    return 0;
}