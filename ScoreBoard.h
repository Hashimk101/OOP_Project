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
void addLevelClear() { scores += LEVEL_CLEAR_POINTS; }  
void addName(std::string n) { name = n; }  

int getScore() const { return scores; }  

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


std::string topTwenty() {
    const int MAX_SCORES = 20;  // We only care about the top 20
    std::string names[MAX_SCORES];
    int scores[MAX_SCORES];
    int count = 0;

    std::ifstream in(filepath);
    if (!in.is_open()) {
        return "Error: Could not open " + filepath + " for reading.\n";
    }

    // Read scores from file (up to MAX_SCORES)
    while (count < MAX_SCORES && in >> names[count] >> scores[count]) {
        count++;
    }
    in.close();

    // Bubble Sort (descending order)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                // Swap scores
                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                // Swap names
                std::string tempName = names[j];
                names[j] = names[j + 1];
                names[j + 1] = tempName;
            }
        }
    }

    // Format the top 20 (or fewer) scores as a string  
    std::string result = "Top 20 Scores:\n";
    for (int i = 0; i < count; i++) {
        result += std::to_string(i + 1) + ". " + names[i] + ": " + std::to_string(scores[i]) + "\n";
    }

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