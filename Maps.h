#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
//#include "Header.h"

//const int cell_size = 64;
const int height = 14;
const int width = 110;

class Maps {
	// only making one map rn so there is one main class over here, else multiple classes based on the levels
	char** lvl;
public:
	Maps() {
		lvl = new char* [height];
		for (int i = 0; i < height; i++) {
			lvl[i] = new char[width];
		}
		for (int i = 0; i < 19 ;i++) {
			lvl[11][i] = 'w';
		}
		lvl[8][4] = 'w';
		lvl[8][5] = 'w';
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
};