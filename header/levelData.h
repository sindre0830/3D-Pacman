#ifndef __LEVELDATA_H
#define __LEVELDATA_H
/* library */
#include "dictionary.h"
#include <vector>
#include <map>
#include <string>
/**
 * @brief Global structure with all level related data
 * 
 */
struct LevelData {
    /* data */
    std::string filePath = "level/";
    int gridWidth, gridHeight, pelletSize, score, pacmanCol, pacmanRow, gamemode = TWO_DIMENSIONAL, windowWidth, windowHeight;
    std::vector<std::vector<int>> grid;
    std::map<std::pair<int, int>, std::vector<std::vector<float>>> gridElement;
    float gridElementWidth, gridElementHeight;
    bool magicEffect = false, scoreChanged = false, gameover = false;
    /* functionality */
    LevelData();
    int getScore(const int index);
    bool inputData();
};

#endif
