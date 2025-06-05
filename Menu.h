#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Audio.h"
#include "Game.h"
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
            text[i].setCharacterSize(40);
            text[i].setOutlineThickness(3);
            text[i].setOutlineColor(sf::Color::Black);
            text[i].setFillColor(sf::Color::White);
            text[i].setPosition(550, 220 + i * 150);
        }

        OptionBanners = new sf::Sprite[numOptions];
        for (int i = 0; i < numOptions; i++)
        {
            OptionBanners[i].setTexture(BannerTxture);
            OptionBanners[i].setScale(0.5, 0.5);
            OptionBanners[i].setPosition(650, 200 + i * 150);
            OptionBanners[i].setOrigin(300, 50);
        }

        SelectedIndex = 0;
        text[SelectedIndex].setFillColor(lightBlue);

        TlTxt = "Sonic Classic Heroes";
        Title.setCharacterSize(110);
        Title.setFont(Monaco);
        Title.setOutlineThickness(5);
        Title.setOutlineColor(sf::Color::Black);
        Title.setFillColor(sf::Color::White);
        Title.setPosition(120, 50);
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
            levelSelectionTitle.setCharacterSize(100);
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
        if (inputDelay.getElapsedTime().asMilliseconds() < 80)
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

    void saveCurrentState(MotoBug* m, CrabMeat* c, BatBrain* b, BuzzBomber* B, EggStinger* E, MySprite* pl, char** lvl, int LEVEL_WIDTH, Scores& s) {

        string path = "Data/currentgamestate.txt";
        ofstream saveFile(path);
        if (!saveFile.is_open()) {
            cout << "Error: Could not create save file at " << path << endl;
            return;
        }

        // Save player info
        saveFile << "PLAYER" << endl;
        saveFile << pl->getX() << " " << pl->getY() << " ";
        saveFile << pl->getHP() << " " << pl->getOffsetX() << " " << pl->getFriction() << " " << pl->getGravity() << endl;

        // MotoBugs
        int activeMoto = 0;
        for (int i = 0; i < 10; ++i)
            if (m[i].isActiveEnemy()) activeMoto++;

        saveFile << "MOTOBUG " << activeMoto << endl;
        for (int i = 0; i < 10; ++i) {
            if (m[i].isActiveEnemy()) {
                saveFile << m[i].getX() << " " << m[i].getY() << " 1" << endl;
            }
        }

        // CrabMeats
        int activeCrab = 0;
        for (int i = 0; i < 10; ++i)
            if (c[i].isActiveEnemy()) activeCrab++;

        saveFile << "CRABMEAT " << activeCrab << endl;
        for (int i = 0; i < 10; ++i) {
            if (c[i].isActiveEnemy()) {
                saveFile << c[i].getX() << " " << c[i].getY() << " 1" << endl;
            }
        }

        // BatBrains
        int activeBat = 0;
        for (int i = 0; i < 10; ++i)
            if (b[i].isActiveEnemy()) activeBat++;

        saveFile << "BATBRAIN " << activeBat << endl;
        for (int i = 0; i < 10; ++i) {
            if (b[i].isActiveEnemy()) {
                saveFile << b[i].getX() << " " << b[i].getY() << " 1" << endl;
            }
        }

        // BuzzBombers
        int activeBuzz = 0;
        for (int i = 0; i < 10; ++i)
            if (B[i].isActiveEnemy()) activeBuzz++;

        saveFile << "BUZZBOMBER " << activeBuzz << endl;
        for (int i = 0; i < 10; ++i) {
            if (B[i].isActiveEnemy()) {
                saveFile << B[i].getX() << " " << B[i].getY() << " 1" << endl;
            }
        }

        // EggStingers
        int activeEgg = 0;
        for (int i = 0; i < 5; ++i)
            if (E[i].isActiveEnemy()) activeEgg++;

        saveFile << "EGGSTINGER " << activeEgg << endl;
        for (int i = 0; i < 5; ++i) {
            if (E[i].isActiveEnemy()) {
                saveFile << E[i].getX() << " " << E[i].getY() << " 1" << endl;
            }
        }

        // Scan level grid for items
        int coinCount = 0;
        int boostCount = 0;
        int lifeCount = 0;

        for (int i = 0; i < 14; ++i) {
            for (int j = 0; j < LEVEL_WIDTH; ++j) {
                if (lvl[i][j] == 'c') coinCount++;
                else if (lvl[i][j] == 'd') boostCount++;
                else if (lvl[i][j] == 'L') lifeCount++;
            }
        }

        // Save RingCoins
        saveFile << "RINGCOIN " << coinCount << endl;
        for (int i = 0; i < 14; ++i) {
            for (int j = 0; j < LEVEL_WIDTH; ++j) {
                if (lvl[i][j] == 'c') {
                    saveFile << j << " " << i << " 1" << endl;
                }
            }
        }

        // Save SpecialBoosts
        saveFile << "SPECIALBOOST " << boostCount << endl;
        for (int i = 0; i < 14; ++i) {
            for (int j = 0; j < LEVEL_WIDTH; ++j) {
                if (lvl[i][j] == 'd') {
                    saveFile << j << " " << i << " 1" << endl;
                }
            }
        }

        // Save ExtraLives
        saveFile << "EXTRALIFE " << lifeCount << endl;
        for (int i = 0; i < 14; ++i) {
            for (int j = 0; j < LEVEL_WIDTH; ++j) {
                if (lvl[i][j] == 'L') {
                    saveFile << j << " " << i << " 1" << endl;
                }
            }
        }

        // Player stats
        saveFile << "STATS" << endl;
        saveFile << s.getScore() << endl;
        saveFile << "LEVEL" << endl;

        for (int i = 0; i < 14; ++i) {
            for (int j = 0; j < LEVEL_WIDTH; ++j) {
                saveFile << lvl[i][j];
            }
            saveFile << '\n'; // new line after each row
        }

        saveFile.close();
        cout << "Game state saved successfully to " << path << endl;
    }

    void LoadOldState(MotoBug* m, CrabMeat* c, BatBrain* b, BuzzBomber* B, EggStinger* E, MySprite* pl, char** lvl, int LEVEL_WIDTH, Scores& s, RingCoin* coins, ExtraLife* diamonds, SpecialBoost* special) {

        string path = "Data/currentgamestate.txt";
        ifstream loadFile(path);
        if (!loadFile.is_open()) {
            cout << "Error: Could not open save file at " << path << endl;
            return;
        }

        string line;

        // Read and ignore the "PLAYER" header
        getline(loadFile, line);

        // Load player data
        int px, py, hp, offsetX, friction, gravity;
        loadFile >> px >> py >> hp >> offsetX >> friction >> gravity;
        pl->setX(px);
        pl->setY(py);
        pl->SetHp(hp);
        pl->setOffsetX(offsetX);
        pl->setFriction(friction);
        pl->setGravity(gravity);

        // Load MotoBugs
        getline(loadFile, line); // MOTOBUG header
        int activeMoto;
        loadFile >> activeMoto;
        for (int i = 0; i < activeMoto; ++i) {
            int mx, my;
            loadFile >> mx >> my;
            m[i].setX(mx);
            m[i].setY(my);
            m[i].activate();
        }

        // Load CrabMeats
        getline(loadFile, line); // CRABMEAT header
        int activeCrab;
        loadFile >> activeCrab;
        for (int i = 0; i < activeCrab; ++i) {
            int cx, cy;
            loadFile >> cx >> cy;
            c[i].setX(cx);
            c[i].setY(cy);
            c[i].activate();
        }

        // Load BatBrains
        getline(loadFile, line); // BATBRAIN header
        int activeBat;
        loadFile >> activeBat;
        for (int i = 0; i < activeBat; ++i) {
            int bx, by;
            loadFile >> bx >> by;
            b[i].setX(bx);
            b[i].setY(by);
            b[i].activate();
        }

        // Load BuzzBombers
        getline(loadFile, line); // BUZZBOMBER header
        int activeBuzz;
        loadFile >> activeBuzz;
        for (int i = 0; i < activeBuzz; ++i) {
            int bx, by;
            loadFile >> bx >> by;
            B[i].setX(bx);
            B[i].setY(by);
            B[i].activate();
        }

        // Load EggStingers
        getline(loadFile, line); // EGGSTINGER header
        int activeEgg;
        loadFile >> activeEgg;
        for (int i = 0; i < activeEgg; ++i) {
            int ex, ey;
            loadFile >> ex >> ey;
            E[i].setX(ex);
            E[i].setY(ey);
            E[i].activate();
        }

       

        // Load Player Stats
        getline(loadFile, line); // STATS header
        int score;
        loadFile >> score;
        s.setScore(score);

        // Load level data
        getline(loadFile, line); // LEVEL header
        for (int i = 0; i < 14; ++i) {
            for (int j = 0; j < LEVEL_WIDTH; ++j) {
                loadFile >> lvl[i][j];
            }
        }

        loadFile.close();
        cout << "Game state loaded successfully from " << path << endl;
    }


    ~Menu()
    {
        delete[] options;
        delete[] text;
        delete[] levelsTxt;
        delete[] OptionBanners;
    }
};