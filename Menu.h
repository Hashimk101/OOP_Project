#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Audio.h"
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

    sf::Text namePrompt, nameText;
    std::string playerName;
    bool nameEntered;

    sf::Color lightBlue;
    sf::Texture BannerTxture;
    sf::Sprite* OptionBanners;
    sf::SoundBuffer MenuBgMusic;
    sf::Sound MenuMus;
    sf::SoundBuffer OptionBuffer;
    sf::Sound OptionScrollSound;

    bool keyHandled = false;
    bool enterPressed = false;

public:
    Menu(sf::RenderWindow& GameWindow) : window(GameWindow), lightBlue(180, 236, 250)
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
        if (!MenuBgMusic.loadFromFile("Data/01-Title Screen.ogg"))
            std::cout << "Failed to load 01-Title Screen.ogg" << std::endl;
        MenuMus.setBuffer(MenuBgMusic);
        MenuMus.setVolume(70);
        MenuMus.setLoop(true);

        if (!OptionBuffer.loadFromFile("Data/MenuButton.wav"))
        {
            std::cout << "Failed to load MenuButton.wav" << std::endl;
        }
        OptionScrollSound.setBuffer(OptionBuffer);
        OptionScrollSound.setVolume(AudioSystem::GameVolume);

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
        Title.setOutlineColor(sf::Color::Black);
        Title.setFillColor(sf::Color::White);
        Title.setPosition(105, 100);
        Title.setString(TlTxt);

        namePrompt.setFont(Monaco);
        namePrompt.setString("Enter Your Name");
        namePrompt.setCharacterSize(60);
        namePrompt.setFillColor(sf::Color::Black);
        namePrompt.setPosition(350, 250);
        nameText.setFont(Monaco);
        nameText.setCharacterSize(60);
        nameText.setFillColor(lightBlue);
        nameText.setPosition(350, 350);
        nameEntered = false; // Initialize nameEntered
    }

    void draw(sf::RenderWindow& GameWindow)
    {
        GameWindow.draw(BackgroundSprite);
        GameWindow.draw(Title);
        for (int i = 0; i < numOptions; i++)
        {
            GameWindow.draw(OptionBanners[i]);
            GameWindow.draw(text[i]);
        }
    }

    void drawNameInput(sf::RenderWindow& GameWindow)
    {
        window.clear();
        GameWindow.draw(BackgroundSprite);
        GameWindow.draw(namePrompt);
        GameWindow.draw(nameText);
    }

    void moveUp()
    {
        if (SelectedIndex < 0 || SelectedIndex >= numOptions) SelectedIndex = 0;
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
        if (SelectedIndex < 0 || SelectedIndex >= numOptions) SelectedIndex = 0;
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

    int selectLevel()
    {
        int totalLevels = 4;
        int selected = 1;

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
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    selecting = false;
                    delete[] Levels;
                    return -1;
                }
                if (event.type == sf::Event::KeyPressed)
                {
                    if (OptionScrollSound.getStatus() != sf::Sound::Playing)
                        OptionScrollSound.play();

                    switch (event.key.code)
                    {
                    case sf::Keyboard::Up:
                        if (selected > 1)
                        {
                            Levels[selected - 1].setFillColor(sf::Color::White);
                            selected--;
                            Levels[selected - 1].setFillColor(lightBlue);
                        }
                        break;
                    case sf::Keyboard::Down:
                        if (selected < totalLevels)
                        {
                            Levels[selected - 1].setFillColor(sf::Color::White);
                            selected++;
                            Levels[selected - 1].setFillColor(lightBlue);
                        }
                        break;
                    case sf::Keyboard::Enter:
                        selecting = false;
                        delete[] Levels;
                        return selected;
                    case sf::Keyboard::Escape:
                        selecting = false;
                        delete[] Levels;
                        return -1;
                    }
                }
                else if (event.type == sf::Event::KeyReleased)
                {
                    OptionScrollSound.stop();
                }
            }

            window.clear();
            window.draw(BackgroundSprite);
            sf::Text levelSelectionTitle;
            levelSelectionTitle.setFont(Monaco);
            levelSelectionTitle.setString("Select Level");
            levelSelectionTitle.setCharacterSize(120);
            levelSelectionTitle.setOutlineThickness(5);
            levelSelectionTitle.setOutlineColor(sf::Color::Black);
            levelSelectionTitle.setFillColor(sf::Color::White);
            levelSelectionTitle.setPosition(350, 100);
            window.draw(levelSelectionTitle);
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

    bool isEnterPressed() const { return enterPressed; }
    int getSelectedIndex() const { return SelectedIndex; }
    void resetEnter() { enterPressed = false; }
    bool isNameEntered() const { return nameEntered; }

    void handleEvent(const sf::Event& event)
    {
        if (nameEntered)
        {
            handleNameInput(event); // Process name input events
        }
        else
        {
            if (event.type == sf::Event::KeyReleased)
            {
                OptionScrollSound.stop();
                keyHandled = false;
            }

            if (event.type == sf::Event::KeyPressed && !keyHandled)
            {
                if (OptionScrollSound.getStatus() != sf::Sound::Playing)
                    OptionScrollSound.play();
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
                    keyHandled = true;
                    enterPressed = true;
                    break;
                default:
                    break;
                }
            }
        }
    }

    void PlayMenuMusic()
    {
        MenuMus.play();
    }

    void StopMenuMusic()
    {
        MenuMus.stop();
    }
    string GetPlayerName() 
    {
        return playerName;
    }
    void enterName()
    {
        nameEntered = true;
        playerName.clear();
        nameText.setString("");
    }

    void handleNameInput(const sf::Event& event)
    {
        static sf::Clock inputDelay;
        if (inputDelay.getElapsedTime().asMilliseconds() < 150)
            return;

        if (event.type == sf::Event::KeyPressed)
        {
            if (playerName.length() < 20)
            {
                if (event.key.code == sf::Keyboard::A) { playerName += 'A'; }
                else if (event.key.code == sf::Keyboard::B) { playerName += 'B'; }
                else if (event.key.code == sf::Keyboard::C) { playerName += 'C'; }
                else if (event.key.code == sf::Keyboard::D) { playerName += 'D'; }
                else if (event.key.code == sf::Keyboard::E) { playerName += 'E'; }
                else if (event.key.code == sf::Keyboard::F) { playerName += 'F'; }
                else if (event.key.code == sf::Keyboard::G) { playerName += 'G'; }
                else if (event.key.code == sf::Keyboard::H) { playerName += 'H'; }
                else if (event.key.code == sf::Keyboard::I) { playerName += 'I'; }
                else if (event.key.code == sf::Keyboard::J) { playerName += 'J'; }
                else if (event.key.code == sf::Keyboard::K) { playerName += 'K'; }
                else if (event.key.code == sf::Keyboard::L) { playerName += 'L'; }
                else if (event.key.code == sf::Keyboard::M) { playerName += 'M'; }
                else if (event.key.code == sf::Keyboard::N) { playerName += 'N'; }
                else if (event.key.code == sf::Keyboard::O) { playerName += 'O'; }
                else if (event.key.code == sf::Keyboard::P) { playerName += 'P'; }
                else if (event.key.code == sf::Keyboard::Q) { playerName += 'Q'; }
                else if (event.key.code == sf::Keyboard::R) { playerName += 'R'; }
                else if (event.key.code == sf::Keyboard::S) { playerName += 'S'; }
                else if (event.key.code == sf::Keyboard::T) { playerName += 'T'; }
                else if (event.key.code == sf::Keyboard::U) { playerName += 'U'; }
                else if (event.key.code == sf::Keyboard::V) { playerName += 'V'; }
                else if (event.key.code == sf::Keyboard::W) { playerName += 'W'; }
                else if (event.key.code == sf::Keyboard::X) { playerName += 'X'; }
                else if (event.key.code == sf::Keyboard::Y) { playerName += 'Y'; }
                else if (event.key.code == sf::Keyboard::Z) { playerName += 'Z'; }
            }

            if (event.key.code == sf::Keyboard::BackSpace && !playerName.empty())
            {
                playerName.pop_back();
            }

            if (event.key.code == sf::Keyboard::Enter && !playerName.empty())
            {
                nameEntered = false;
            }

            nameText.setString(playerName);
            inputDelay.restart();
        }
    }

    ~Menu()
    {
        delete[] options;
        delete[] text;
        delete[] levelsTxt;
        delete[] OptionBanners;
    }
};