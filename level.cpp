/* library */
#include "header/level.h"
#include "header/dictionary.h"
#include <iostream>
#include <fstream>
#include <algorithm>
/**
 * @brief Construct a new Level Data:: Level Data object
 * 
 */
LevelData::LevelData() {
	//set window size
	windowHeight = 1024;
	windowWidth = 1024;
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
bool LevelData::inputData(const int index) {
	//set file pathing values
    std::string filePath = "level/";
    std::vector<std::string> possibleLevelsArr = {"level0", "level1"};
	//get file data from user input
	filePath.append(possibleLevelsArr[index]);
	//read from file
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
		//get offsets
		if(gridWidth < gridHeight) {
			rowOffset = gridHeight - gridWidth;
		} else colOffset = gridWidth - gridHeight;
		//
		if(rowOffset % 2 || colOffset % 2) return false;
		//declare a vector filled with VOID to be used incase of colOffsets
		std::vector<int> rowVOID(gridWidth + rowOffset, VOID);
		for(int i = 0; i < colOffset / 2; i++) {
			grid.push_back(rowVOID);
		}
		//go through file and add values to grid vector
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
			for(int i = 0; i < rowOffset / 2; i++){
				arrRow.insert(arrRow.begin(), VOID);
				arrRow.insert(arrRow.end(), VOID);
			}
			grid.push_back(arrRow);
		}
		file.close();
		for(int i = 0; i < colOffset / 2; i++) {
			grid.push_back(rowVOID);
		}
		//set correct width and hight values
		gridWidth += rowOffset;
		gridHeight += colOffset;
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
		return true;
	} else return false;
}
