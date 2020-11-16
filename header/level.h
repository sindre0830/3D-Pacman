#ifndef __LEVEL_H
#define __LEVEL_H
/* library */
#include "dictionary.h"
#include <vector>
#include <map>
/**
 * @brief Global structure with all level related data
 * 
 */
struct Level {
    /* data */
    const int windowWidth = 1024, windowHeight = 1024;
    int rowOffset = 0, colOffset = 0, gridWidth = 0, gridHeight = 0;
    int pelletSize = 0, score = 0, gamemode = THIRD_PERSON, pacmanCol = 0, pacmanRow = 0;
    std::vector<std::vector<int>> grid;
    std::map<std::pair<int, int>, std::vector<std::vector<float>>> gridElement;
    float gridElementWidth = 0.f, gridElementHeight = 0.f;
    bool magicEffect = false, scoreChanged = false, gameover = false;
    /* functionality */
    int getScore(const int index);
    bool inputData(const int index);
};

#endif
