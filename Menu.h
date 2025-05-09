#pragma once  
#include <iostream>  
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;

class Menu
{
private:
    sf::Font Monaco;
    sf::Texture BackgroundTex;
    sf::Sprite BackgroundSprite;
    sf::RenderWindow& window;
    sf::Text* text;
    sf::Text* Levels;
    sf::Text Title;
    sf::String* options;
    sf::String* levelsTxt;
    string TlTxt;
    int SelectedIndex;
    static const int numOptions = 5;
    bool keyHandled = false;
    sf::Color lightBlue;
    sf::Texture BannerTxture;
    sf::Sprite* OptionBanners;

public:
    Menu(RenderWindow& GameWindow) : window(GameWindow), lightBlue(180, 236, 250)
    {
        if (!Monaco.loadFromFile("Data/Tricky Jimmy.ttf")) 
        {
            std::cout << "Error loading font file: Data/Tricky Jimmy.ttf" << std::endl;
           
        }
        if (!BackgroundTex.loadFromFile("Data/menubg.png")) 
        {
            std::cout << "Error loading background texture: Data/menubg.png" << std::endl;
            
        }
        if (!BannerTxture.loadFromFile("Data/IDK.png"))
        {
            std::cout << "Error loading banner texture: Data/IDK.png" << std::endl;
            
        }

        BackgroundSprite.setTexture(BackgroundTex);
        BackgroundSprite.setScale(2, 2.25);

        options = new sf::String[numOptions];
        options[0] = "New Game";
        options[1] = "Resume";
        options[2] = "Options";
        options[3] = "High Scores";
        options[4] = "Exit";

        levelsTxt = new sf::String[4];
        levelsTxt[0] = "Level 1";
        levelsTxt[1] = "Level 2";
        levelsTxt[2] = "Level 3";
        levelsTxt[3] = "Final Boss";

        text = new sf::Text[numOptions];
        for (int i = 0; i < numOptions; i++)
        {
            text[i].setFont(Monaco);
            text[i].setString(options[i]);
            text[i].setCharacterSize(60);
			text[i].setOutlineThickness(3);
			text[i].setOutlineColor(sf::Color::Black);
            text[i].setFillColor(sf::Color::White);
            text[i].setPosition(450, 325 + i * 150);
        }

        OptionBanners = new sf::Sprite[numOptions];
        for (int i = 0; i < numOptions; i++)
        {
            OptionBanners[i].setTexture(BannerTxture);
            OptionBanners[i].setScale(0.75, 0.75);
            OptionBanners[i].setPosition(650, 300 + i * 150);
            OptionBanners[i].setOrigin(385, 50);
        }

        SelectedIndex = 0;
        text[SelectedIndex].setFillColor(lightBlue);

        TlTxt = "Sonic Classic Heroes";
        Title.setCharacterSize(120);
        Title.setFont(Monaco);
        Title.setOutlineThickness(5);
		Title.setOutlineColor(sf::Color::Cyan);
        Title.setFillColor(sf::Color::White);
        Title.setPosition(105, 100);
        Title.setString(TlTxt);
    }

    void draw(RenderWindow& GameWindow)
    {
        GameWindow.draw(BackgroundSprite);
        GameWindow.draw(Title);
        for (int i = 0; i < numOptions; i++)
        {
            GameWindow.draw(OptionBanners[i]);
            GameWindow.draw(text[i]);
        }
    }

    void moveUp()
    {
        text[SelectedIndex].setFillColor(sf::Color::White);

        if (SelectedIndex == 0)
        {
            SelectedIndex = numOptions - 1;
        }
        else
        {
            SelectedIndex--;
        }
        text[SelectedIndex].setFillColor(lightBlue);
    }

    void moveDown()
    {
        text[SelectedIndex].setFillColor(sf::Color::White);
        if (SelectedIndex == numOptions - 1)
        {
            SelectedIndex = 0;
        }
        else
        {
            SelectedIndex++;
        }
        text[SelectedIndex].setFillColor(lightBlue);
    }

    int selectLevel() {
        int totalLevels = 4; 
        int selected = 1;

        // Create and initialize level selection text objects
        Levels = new sf::Text[totalLevels];
        for (int i = 0; i < totalLevels; i++)
        {
            Levels[i].setFont(Monaco);
            Levels[i].setString(levelsTxt[i]);
            Levels[i].setCharacterSize(60);
			Levels[i].setOutlineThickness(3);
			Levels[i].setOutlineColor(sf::Color::Black);
            Levels[i].setFillColor(i == selected - 1 ? lightBlue : sf::Color::White);
            Levels[i].setPosition(450, 325 + i * 150);
        }

        bool selecting = true;
        while (selecting && window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    selecting = false;
                    delete[] Levels;
                    return -1;
                }
                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                    case sf::Keyboard::Left:
                        if (selected > 1) {
                            Levels[selected - 1].setFillColor(sf::Color::White);
                            selected--;
                            Levels[selected - 1].setFillColor(lightBlue);
                        }
                        break;
                    case sf::Keyboard::Right:
                        if (selected < totalLevels) {
                            Levels[selected - 1].setFillColor(sf::Color::White);
                            selected++;
                            Levels[selected - 1].setFillColor(lightBlue);
                        }
                        break;
                    case sf::Keyboard::Enter:
                        selecting = false;
                        break;
                    case sf::Keyboard::Escape:
                        selecting = false;
                        delete[] Levels;
                        return -1; // canceled
                    }
                }
            }

            window.clear();
            window.draw(BackgroundSprite);

            // Draw title for level selection
            sf::Text levelSelectionTitle;
            levelSelectionTitle.setFont(Monaco);
            levelSelectionTitle.setString("Select Level");
            levelSelectionTitle.setCharacterSize(100);
			levelSelectionTitle.setOutlineThickness(5);
			levelSelectionTitle.setOutlineColor(sf::Color::Black);
            levelSelectionTitle.setFillColor(sf::Color::White);
            levelSelectionTitle.setPosition(400, 200);
            window.draw(levelSelectionTitle);

            // Draw all level options
            for (int i = 0; i < totalLevels; i++) {
                window.draw(Levels[i]);
            }

            window.display();
        }

        int result = selected;
        delete[] Levels;
        Levels = nullptr;
        return result;
    }

    bool startLevel(int levelNumber) {
        std::cout << "Loading Level " << levelNumber << "..." << std::endl;

        // Replace with real level loading logic
        sf::sleep(sf::seconds(2)); // simulate level load

        // Placeholder: always return true (success)
        return true;
    }

    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed && !keyHandled)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                moveUp();
                keyHandled = true;
                break;
            case sf::Keyboard::Down:
                moveDown();
                keyHandled = true;
                break;

            case sf::Keyboard::Enter:
                switch (SelectedIndex)
                {
                case 0: // New Game
                {
                    std::cout << "Starting a new game..." << std::endl;
                    int chosenLevel = selectLevel();
                    if (chosenLevel > 0) {
                        bool completed = startLevel(chosenLevel);
                        if (completed) 
                        {
                            std::cout << "Level " << chosenLevel << " completed!" << std::endl;
                        }
                        else {
                            std::cout << "Game over on Level " << chosenLevel << "." << std::endl;
                        }
                    }
                }
                break;

                case 1: // Resume
                    std::cout << "Resuming game..." << std::endl;
                    // Add logic to resume the game
                    break;
                case 2: // Options
                    std::cout << "Opening options..." << std::endl;
                    // Add logic to open options menu
                    break;
                case 3: // High Scores
                    std::cout << "Opening high scores..." << std::endl;
                    // Add logic to open high scores
                    break;
                case 4: // Exit
                    std::cout << "Exiting game..." << std::endl;
                    window.close();
                    break;
                }
                keyHandled = true;
                break;
            default:
                break;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            keyHandled = false; // reset
        }
    }

    ~Menu() {
        delete[] options;
        delete[] text;
        delete[] levelsTxt;
        delete[] OptionBanners;
        // No need to delete Levels as it's already handled in selectLevel()
    }
};