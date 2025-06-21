#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
//#include "Header.h"

//const int cell_size = 64;
//Heights of all levels are same



class Maps {
     int widthLvl;
    // only making one map rn so there is one main class over here, else multiple classes based on the levels
    char** lvl;
    int level;
    std::string* FilePath = nullptr;
    std::string File = "";
    int height = 14;
public:
Maps() : lvl(nullptr), widthLvl(0), level(0), FilePath(nullptr), File("")
{

}
    Maps(int levl) : level(levl)
        , widthLvl((levl == 1) ? 200
            : (levl == 2) ? 250
            : (levl == 3) ? 300
            : 200)
    {

        lvl = new char* [height];
        for (int i = 0; i < height; i++)
        {
            lvl[i] = new char[widthLvl];
            // Initialize all cells to empty space
            for (int j = 0; j < widthLvl; j++)
            {
                lvl[i][j] = 's'; // 's' for empty space
            }
        }
        FilePath = new std::string[4];
        FilePath[0] = "Data/level1.txt";
        FilePath[1] = "Data/level2.txt";
        FilePath[2] = "Data/level3.txt";
        FilePath[3] = "Data/Boss_Level.txt";
        this->level = levl;

        File = (levl == 1) ? FilePath[0] : ((levl == 2) ? FilePath[1] : (levl == 3) ? FilePath[2] : FilePath[3]);


        std::ifstream file(File);
        if (!file.is_open())
        {
            std::cerr << "Failed to open map file: " << File << std::endl;
            // Continue with hardcoded map if file doesn't exist
        }
        else {

            std::string line;
            int row = 0;

            while (std::getline(file, line) && row < height) {
                for (int col = 0; col < line.length() && col < widthLvl; col++)
                {
                    lvl[row][col] = line[col];
                }
                row++;
            }

            file.close();
            std::cout << "Map loaded from file: " << File << std::endl;

        }







    }





    Maps(Maps& m) : widthLvl(m.widthLvl), level(m.level), File(m.File), height(m.height)
    {
        lvl = new char* [height];
        for (int i = 0; i < height; i++) {
            lvl[i] = new char[widthLvl];
            for (int j = 0; j < widthLvl; j++)
            {
                lvl[i][j] = m.lvl[i][j];
            }
        }
        FilePath = new std::string[4];
        for (int i = 0; i < 4; i++) {
            FilePath[i] = m.FilePath[i];
        }
    }
	//Maps& operator=(const Maps& m) {
	//	if (this != &m) {
	//		for (int i = 0; i < height; i++) {
	//			delete[] lvl[i];
	//		}
	//		delete[] lvl;
	//		widthLvl = m.widthLvl;
	//		level = m.level;
	//		height = m.height;
	//		File = m.File;
	//		lvl = new char* [height];
	//		for (int i = 0; i < height; i++) {
	//			lvl[i] = new char[widthLvl];
	//			for (int j = 0; j < widthLvl; j++)
	//			{
	//				lvl[i][j] = m.lvl[i][j];
	//			}
	//		}
	//	}
	//	return *this;
	//}
    char** getMap() {
        return lvl;
    }
    ~Maps() {
        for (int i = 0; i < height; i++) {
            delete[] lvl[i];
        }
        delete[] lvl;
    }
    /*void setLevel(int a, int b) {

    }*/

    const int GetLevelWidth() const
    {
        return widthLvl;
    }
};