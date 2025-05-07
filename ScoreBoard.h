#pragma once
#include <fstream>
#include <string>
#include <iostream>

class Scores {
	/* each coin is worth 10 points
	* each kill of a motobug is worth 40 points
	* each kill of a crabmeat is worth 60 points
	* each kill of a batbrain is worth 60 points
	* each kill of a buzzbomber is worth 70 points
	* passing a level will give 200 points
	*/
;
	const int COIN_POINTS = 10;
	const int MOTOBUG_POINTS = 40;
	const int CRABMEAT_POINTS = 60;
	const int BATBRAIN_POINTS = 60;
	const int BUZZBOMBER_POINTS = 70;
	const int LEVEL_CLEAR_POINTS = 200;
	std::string filepath;
	std::string name;
	int scores;
public:
	Scores()
	{
		scores = 0;
		filepath = "Data/scores.txt";
		//std::fstream fileopen(filepath, std::ios::app);

	}
	void addCoin() { scores += COIN_POINTS; }
	void addMotoBugKill() { scores += MOTOBUG_POINTS; }
	void addCrabMeatKill() { scores += CRABMEAT_POINTS; }
	void addBatBrainKill() { scores += BATBRAIN_POINTS; }
	void addBuzzBomberKill() { scores += BUZZBOMBER_POINTS; }
	void addLevelClear() { scores += LEVEL_CLEAR_POINTS; }
	void addName(std::string n) { name = n; }

	int getScore() const { return scores; }

	void saveScore() const{
		std::ofstream out(filepath, std::ios::app);
		if (out.is_open()) {
			out << name << " " << scores << "\n";
			out.close();
		}
		else {
			std::cout  << "Error: Could not open " << filepath << " for writing.\n";
		}

	}


    std::string topTen() {
        const int MAX_SCORES = 100; // Maximum number of scores we'll read from file
        std::string names[MAX_SCORES];
        int scores[MAX_SCORES];
        int count = 0;

        std::ifstream in(filepath);
        if (!in.is_open()) {
            return "Error: Could not open " + filepath + " for reading.\n";
        }

        // Read scores from file
        while (count < MAX_SCORES && in >> names[count] >> scores[count]) {
            count++;
        }
        in.close();

        // Sort scores using bubble sort (descending order)
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

        // Format the top 10 scores as a string
        std::string result = "Top 10 Scores:\n";
        int displayCount = (count < 10) ? count : 10; // Display at most 10 scores

        for (int i = 0; i < displayCount; i++) {
            result += std::to_string(i + 1) + ". " + names[i] + ": " + std::to_string(scores[i]) + "\n";
        }

        return result;
    }
};