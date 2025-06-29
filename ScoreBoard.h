#pragma once  
#include <fstream>  
#include <string>  
#include <iostream>  
#include <SFML/Graphics.hpp>  
#include <SFML/Audio.hpp>  
#include <SFML/Window.hpp>  
using namespace std;
using namespace sf;
class Scores  
{  
/* each coin is worth 10 points  
* each kill of a motobug is worth 40 points  
* each kill of a crabmeat is worth 60 points  
* each kill of a batbrain is worth 60 points  
* each kill of a buzzbomber is worth 70 points  
* passing a level will give 200 points  
*/  
const int COIN_POINTS = 10;  
const int MOTOBUG_POINTS = 40;  
const int CRABMEAT_POINTS = 60;  
const int BATBRAIN_POINTS = 60;  
const int BUZZBOMBER_POINTS = 70;  
const int LEVEL_CLEAR_POINTS = 200;  
std::string filepath;  
std::string name;  
int scores;  
//For displaying score, make text and string objects  
std::string* DisplayScore=nullptr;  

sf::Text* DisplayTxt= nullptr;  
sf::Font Monaco;  
sf::RenderWindow& w;  

public:  
Scores(RenderWindow& window) : w(window)  
{  
	scores = 0;  
	filepath = "Data/scores.txt";  
	//std::fstream fileopen(filepath, std::ios::app);  
	DisplayScore = new string[2];  
	DisplayScore[0] = "Score: ";  
	DisplayScore[1] = "HP : ";  
	DisplayTxt = new Text[2];  
	if (!Monaco.loadFromFile("Data/Tricky Jimmy.ttf"))  
	{  
		std::cout << "Error loading font file: Data/Tricky Jimmy.ttf" << std::endl;  
	}  


}  
void addCoin() { scores += COIN_POINTS; }  
void addMotoBugKill() { scores += MOTOBUG_POINTS; }  
void addCrabMeatKill() { scores += CRABMEAT_POINTS; }  
void addBatBrainKill() { scores += BATBRAIN_POINTS; }  
void addBuzzBomberKill() { scores += BUZZBOMBER_POINTS; }  
void addBossKill() { scores += 1000; }
void addLevelClear() { scores += LEVEL_CLEAR_POINTS; }  
void addName(std::string n) { name = n; }  

int getScore() const { return scores; }  
void setScore(int a) {
	scores = a;
}

void saveScore() const  
{  
	std::ofstream out(filepath, std::ios::app);  
	if (out.is_open()) {  
		out << name << " " << scores << "\n";  
		out.close();  
	}  
	else {  
		std::cout  << "Error: Could not open " << filepath << " for writing.\n";  
	}  

}  
void SetScore(int escore)
{
    scores = escore;
}

std::string topTwenty() {
    const int MAX_DISPLAY = 20;  // Only display top 20
    std::vector<std::string> names;
    std::vector<int> scores;

    std::ifstream in(filepath);
    if (!in.is_open()) {
        return "Error: Could not open " + filepath + " for reading.\n";
    }

    // Read ALL scores from file
    std::string name;
    int score;
    while (in >> name >> score) {
        names.push_back(name);
        scores.push_back(score);
    }
    in.close();

    if (names.empty()) {
        return "No scores found in file.\n";
    }

    // Sort ALL entries by score (descending order)
    // Using selection sort for simplicity, but you could use std::sort for better performance
    for (int i = 0; i < names.size() - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < names.size(); j++) {
            if (scores[j] > scores[maxIdx]) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            // Swap scores
            std::swap(scores[i], scores[maxIdx]);
            // Swap names
            std::swap(names[i], names[maxIdx]);
        }
    }

    // Format the top 20 (or fewer) scores as a string
    std::string result = "Top 20 Scores:\n";
    int displayCount = std::min(MAX_DISPLAY, (int)names.size());

    for (int i = 0; i < displayCount; i++) {
        result += std::to_string(i + 1) + ". " + names[i] + ": " + std::to_string(scores[i]) + "\n";
    }

    cout << "RESULT" << result << endl;
    return result;
}

    void DisplayScoreWin(MySprite& currentPlayer)   
    {  
	    DisplayScore[0] = "Score : " + to_string(scores);
	    DisplayScore[1] = "HP : " + to_string(currentPlayer.GetHp());
	    for (int i = 0; i < 2; i++)  
	    {  
		    DisplayTxt[i].setFillColor(sf::Color::White); 
		    DisplayTxt[i].setOutlineThickness(7);
		    DisplayTxt[i].setOutlineColor(sf::Color::Black);
		    DisplayTxt[i].setFont(Monaco);  
		    DisplayTxt[i].setString(DisplayScore[i]);  
		    DisplayTxt[i].setCharacterSize(50);  
		    DisplayTxt[i].setPosition(10, 0 + i * 65);  
		    w.draw(DisplayTxt[i]);  
	    }  
    }  

~Scores() {  // FIX: Destructor to prevent memory leaks
    delete[] DisplayScore;
    delete[] DisplayTxt;
}
};