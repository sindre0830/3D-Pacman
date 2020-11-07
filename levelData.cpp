/* library */
#include "header/levelData.h"
#include "header/dictionary.h"
#include <iostream>
#include <fstream>
#include <algorithm>
/**
 * @brief Construct a new Level Data:: Level Data object
 * 
 */
LevelData::LevelData() {
    int index = 0;
    bool flag = true;
    //array with all possible levels
    std::vector<std::string> possibleLevelsArr = {"level1"/*, "level1"*/};
    //branch if there is only 1 level in the array
    if(possibleLevelsArr.size() > 1) {
        //print out all possible levels to the terminal
        std::cout << "Choose a level:\t";
        for(int i = 0; i < possibleLevelsArr.size(); i++) {
            std::cout << i << (i < possibleLevelsArr.size() - 1 ? ", " : "\n");
        }
        //loop until user has inputed a valid integer 
        do {
            std::cout << "Level(0-" << possibleLevelsArr.size() - 1 << "): ";
            std::cin >> index;
            //branch if integer is valid
            if(index <= possibleLevelsArr.size() - 1) flag = false;
        } while(flag);
        std::cout << std::endl;
    }
    //append filename to the filepath
    filePath.append(possibleLevelsArr[index]);
}
/**
 * @brief Converts score to string and makes sure it has as many characters as there are numbers in scoreboard
 * 
 * @param index 
 * @return int 
 */
int LevelData::getScore(const int index) {
    std::string str = "0000";
    std::string buffer = std::to_string(score);
    //branch if buffer has less integers then 1000
    if(buffer.size() < str.size()) {
        //get the difference
        int num = str.size() - buffer.size();
        str = "";
        //append as many integers as was missing in buffer
        for(int i = 0; i < num; i++) {
            str.append("0");
        }
        //append buffer
        str.append(buffer);
    } else str = buffer;
    //reverse string
    std::reverse(str.begin(), str.end());
    return (int)(str[index]) - '0';
}
/**
 * @brief inputs data from level file.
 * 
 */
bool LevelData::inputData() {
    std::ifstream file;
	file.open(filePath);
    //branch if file is open else end program
	if (file) {
		int buffer;
        //input grid width from file
		file >> gridWidth;
		file.ignore();
        //input grid height from file
		file >> gridHeight;
		file.ignore();

		std::cout << gridWidth << ", " << gridHeight << "\n";

		for (int i = 0; i < gridHeight; i++) {
			std::vector<int> arrRow;
			for (int j = 0; j < gridWidth; j++) {
                //input element type from file
				file >> buffer;
                //branch if element is a pellet
				if (buffer == PELLET) {
					//branch if position is a teleportation entrence else increase pellet size
					if(i == 0 || i == gridHeight - 1 || j == 0 || j == gridWidth - 1) {
                        //set element to empty
						buffer = EMPTY;
					} else pelletSize++;
				}
				arrRow.push_back(buffer);
				file.ignore();
			}
			grid.push_back(arrRow);
		}
		file.close();
		//set top left magicpellet
		for (int i = 0, j = 0; i < gridHeight && j < gridWidth; i++, j++) {
			if(grid[i][j] == PELLET) {
				grid[i][j] = MAGICPELLET;
				break;
			}
		}
		//set bottom right magicpellet
		for (int i = gridHeight - 1, j = gridWidth - 1; i >= 0 && j >= 0; i--, j--) {
			if(grid[i][j] == PELLET) {
				grid[i][j] = MAGICPELLET;
				break;
			}
		}
		//reverse order of array
		std::reverse(grid.begin(), grid.end());
		//set element value
		gridElementWidth = 1.f / ((float)(gridWidth) / 2.f);
		gridElementHeight = 1.f / ((float)(gridHeight) / 2.f);
		//fills in map based on column and row (key) with a 2D vector of each corner coordinate (value)
		float
			x = -1.f,
			y = -1.f;
		for (int i = 0; i < gridHeight; i++, x = -1.f, y += gridElementHeight) {
			for (int j = 0; j < gridWidth; j++, x += gridElementWidth) {
				//top left
				gridElement[std::make_pair(i, j)].push_back({x, y + gridElementHeight});
				//bottom left
				gridElement[std::make_pair(i, j)].push_back({x, y});
				//bottom right
				gridElement[std::make_pair(i, j)].push_back({x + gridElementWidth, y});
				//top right
				gridElement[std::make_pair(i, j)].push_back({x + gridElementWidth, y + gridElementHeight});
			}
		}
		//set window size
		windowHeight = gridHeight * 30;
		windowWidth = gridWidth * 30;
		return true;
	} else return false;
}
