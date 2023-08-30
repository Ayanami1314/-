#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <iostream>
#include "field.h"
extern bool debugMode;

// Load map
Field *loadmap(std::istream &is);
// Units move
void GameLogic(std::stringstream &is, Field *f, bool side);
// Main loop for playing the game
void play(Field &field, std::istream &is, std::ostream &os);
bool isGameOver(Field &f);
int whoWin(Field &f); // return 1 if player1 win, -1 if player2, 0 if draw
#endif                // ENGINE_H_INCLUDED
