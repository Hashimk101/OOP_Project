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
    const  int widthLvl;
    // only making one map rn so there is one main class over here, else multiple classes based on the levels
    char** lvl;
    int level;
   std:: string* FilePath=nullptr;
   std:: string File="";
   int height = 14;
public:
   
    Maps(int levl) : widthLvl((levl == 1) ? 200 : ((levl == 2) ? 250 : 300))
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
        FilePath = new std::string[3];
        FilePath[0] = "Data/level1.txt";
        FilePath[1] = "Data/level2.txt";
        FilePath[2] = "Data/level3.txt";
        this->level = levl;

        File = (levl == 1) ? FilePath[0] : ((levl == 2) ? FilePath[1] : FilePath[2]);


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



        //      // Create the ground floor 
        //      for (int i = 0; i < 300; i++)
        //      {
        //          lvl[13][i] = 'w';
        //          lvl[12][i] = 'w';
        //         
        //      }
        //      lvl[11][30] = 'w';
        //      lvl[11][58] = 'K';
        //      lvl[5][98] = 'W';
        //      lvl[11][127] = 'P';


        //      // Fixed platforms (manually placed)
        //      // Small platforms
        //      lvl[8][4] = 'w';
        //      lvl[8][5] = 'w';
        //      lvl[8][6] = 'w';
        //      lvl[8][6] = 'w';

        //      lvl[5][68] = 'K';


        //      lvl[9][107] = 'w';
        //      lvl[8][107] = 'w';
        //      lvl[10][107] = 'w';
        //      lvl[11][107] = 'w';
        //      lvl[8][108] = 'w';
        //      lvl[8][109] = 'w';

        //      //BIGG AA
        //      for (int i = 107; i < 150; i++)
        //      {
        //       
        //          if (i == 110 || i == 120 || i == 130 || i == 140 || i == 149) 
        //          {
        //              lvl[8][i] = 'J';
        //              continue;
        //              if (i == 139) 
        //              {
        //                  lvl[7][i] = 'V';
        //              }
        //          }
        //          lvl[8][i] = 'w';
        //      }



        //      lvl[11][10] = 'P';

        //      lvl[7][10] = 'w';
        //      lvl[7][11] = 'w';
        //      lvl[7][12] = 'w';

        //      lvl[5][20] = 'w';
        //      lvl[5][21] = 'w';
        //      lvl[5][22] = 'w';

        //      // Medium platforms
        //      for (int i = 30; i <= 34; i++)lvl[6][i] = 'w';
        //      for (int i = 50; i <= 54; i++) lvl[9][i] = 'w';
        //      for (int i = 57; i <= 60; i++) lvl[8][i] = 'w';

        //      // Long platforms floating vertiacally
        //      for (int i = 64; i <= 66; i++) lvl[5][i] = 'w';
        //      lvl[6][67] = 'w';lvl[6][68] = 'w';lvl[6][69] = 'w';

        //      for (int i = 70; i <= 74; i++) lvl[4][i] = 'w';
        //      for (int i = 90; i <= 94; i++) lvl[7][i] = 'w';

        //      // Staircase pattern
        //      lvl[5][120] = 'w'; lvl[5][121] = 'w';
        //      lvl[6][123] = 'w'; lvl[6][124] = 'w';
        //      lvl[7][126] = 'w'; /*lvl[7][127] = 'w';*/

        // 
        //      for (int i = 154; i <= 156; i++) {
        //          lvl[4][i] = 'w';
        //          lvl[7][i] = lvl[8][i] = lvl[9][i] = lvl[10][i] = lvl[11][i] = 'b';
        //      }




        //      for (int i = 0; i < widthLvl; i += 8) {
        //          lvl[11][i] = 'r';
        //      }


        //      lvl[10][24] = 'J';
        //      // Floating islands
        //      for (int i = 140; i <= 143; i++)
        //      {
        //          lvl[5][i] = 'w';

        //          
        //      };
        //      //spikes
        //      lvl[11][166] = 'k';
        // 
        //      lvl[11][50] = 'k';
        //      lvl[11][100] = 'k';
        //      lvl[11][150] = 'k';
        //      lvl[11][230] = 'k';
        //      //Crystals
              //lvl[5][159] = 'K';
        //      lvl[6][93] = 'V';
        //      lvl[6][180] = 'W';
        //      lvl[5][186] = 'P';
        //      for (int i = 148; i <= 150; i++) lvl[4][i] = 'w';
        //      for (int i = 155; i <= 160; i++) lvl[6][i] = 'w';
        //      for (int i = 170; i <= 180; i++) lvl[9][i] = 'w';
        //      for (int i = 180; i <= 183; i++) lvl[7][i] = 'w';
        //      for (int i = 186; i <= 190; i++) lvl[6][i] = 'w';

        //      for (int i = 189; i <= 191; i++)
        //      {
        //          
        //          lvl[4][i] = 'w';
        //          if (i == 194) 
        //          {
              //		lvl[4][i] = 'U';
              //		continue;
        //          }
        //          
        //         
        //      }
        //      //breakable walls
        //      for (int i = 0; i < 12; i++)
        //      {
        //        
        //        //  lvl[i][169] = 'b';
        //      }


        //      for (int i = 191; i <= 195; i++) lvl[6][i] = 'w';

        //      // Vertical pillars
        //      //Partially a staircase
        //      lvl[11][15] = 'w';
        //      lvl[11][16] = 'w'; lvl[10][16] = 'w';
        //      lvl[10][17] = 'w'; lvl[9][17] = 'w';lvl[11][17] = 'w';
        //      lvl[8][18] = 'w'; lvl[9][18] = 'w'; lvl[10][18] = 'w';lvl[11][18] = 'w';
        //      lvl[10][45] = 'w'; lvl[9][45] = 'w'; lvl[11][45] = 'w';
        //      lvl[10][79] = 'w'; lvl[9][79] = 'w'; lvl[11][79] = 'w';
        //      //Staircase for upper platforms
        //      lvl[11][78] = 'w'; lvl[10][78] = 'w';
        //      lvl[11][77] = 'w';
        //      //Vertical
        //      for (int i = 80; i <= 89; i++) lvl[7][i] = 'w';
        //      lvl[10][45] = 'w'; lvl[9][45] = 'w';
        //      lvl[10][100] = 'w'; lvl[9][100] = 'w';lvl[11][100] = 'w';
              //lvl[7][144] = 'U';

        //      // Ceiling platforms
        //    
        //      for (int i = 90; i <= 100; i++)
        //      {
        //          lvl[6][i] = 'w';
        //          
        //      }
        //      for (int i = 102; i < 105; i++)
        //      {
        //          lvl[7][i] = 'w';
        //      }

        //      for (int i = 130; i <= 135; i++) lvl[7][i] = 'w';

        //      //Crystals in the end
              //lvl[11][193] = 'K';
              //lvl[11][199] = 'W';



        //  }




    }





    Maps(Maps& m) : widthLvl(m.widthLvl)   
    {
        lvl = new char* [height];
        for (int i = 0; i < height; i++) {
            lvl[i] = new char[widthLvl];
            for (int j = 0; j < widthLvl; j++)
            {
                lvl[i][j] = m.lvl[i][j]; // Copy the map data  
            }
        }
    }
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