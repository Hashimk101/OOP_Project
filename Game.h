// Game.h
#pragma once
#include <fstream>
#include "Header.h"
#include "Maps.h"
#include "Collectables.h"
#include "Enemies.h"
#include "Menu.h"
#include "ScoreBoard.h"
#include "Timer.h"
#include "Audio.h"
#include <SFML/Graphics.hpp>

    /////////////////////////////////////////////////////////////////
    // a cell is 64 by 64 pixels
    //'j' is for jelly
    // 's' is regular space
    // 'q' is wall1 or floor1
    // 'w' is wall2 or floor2
    // 'e' is wall3 or floor3
    // 'b' is breakable wall
    // 'z' is spring

    // Uppercase for not interactable background accessories
    // LVL:1
    //B is for greenBushes
    //T is for brown tower
    // LVL 2
    // R is rocks
    // C is crystals
    //LVL 3
    //K is black crystals
    //P is pink crystals
    //W is white crystals
    //L IS BLUE CRYSTALS
    //V IS VOILET





class Game
{
private:
    // Window settings
    const int screen_x = 1200;
    const int screen_y = 900;
    const int MAX_LEVELS = 4;
    int currentLevel;
    // Game objects
    Maps* map;
    MySprite* players[4];
    int currentPlayer;
    MySprite* player;
    //Knuckles player;
    char** lvl;

    // Collectables
    RingCoin* coins;
    ExtraLife* diamonds;
    SpecialBoost* special;

    // Enemies
    MotoBug* motoBugs;
    int MotobugCount;

    CrabMeat* crabs;
    int CrabCount;

    BatBrain* bats;
    int BatCount;

    BuzzBomber* buzzers;
    int buzzerCount;

    EggStinger* EgStinger;
    int EggStCount;
    // Enemy textures
    sf::Texture motoBugTex;
    sf::Texture crabTex;
    sf::Texture batBrainTex;
    sf::Texture buzzBomberTex;
    sf::Texture eggStingerTex;
    sf::Texture MeatBallTex;
    sf::Texture Projectile;
    //Musics
    AudioSystem audio;

    // Sprites and textures
    sf::Texture backgroundTex;
    sf::Sprite backgroundSprite;
    sf::Texture wallTex1;
    sf::Texture breakableWallTex;
    //level:1
    sf::Texture greenBushTex;
    sf::Texture brownTowerTex;
    sf::Texture spikeTex;
    //GENERAL SHI
    sf::Sprite wallSprite1;
    sf::Sprite breakableWallSprite;
    sf::Texture CompleteLevelBoardTex;
    sf::Sprite CompleteLevelBoardSprite;

    sf::Texture levelBackgroundTexture;
    sf::Sprite levelBackgroundSprite;


    //Level 1
    sf::Sprite brownTowerSprite;
    sf::Sprite greenBushSprite;
    //Level:2
    Texture RockTex1, CrystalTex, JellyTex;
    Sprite Rock1, Crystal1, Jelly;
    //Level:3
    sf::Texture BlackCrystalTex, PinkCrystalTex, WhiteCrystalTex, blueCrystalTex, voiletCrystalTex;
    sf::Sprite BlackCrystal, PinkCrystal, WhiteCrystal, BlueCrystal, voiletCrystal;
    //Boss Level

    //Common obstackle
    sf::Sprite spikeSprite;
    //Level Text 
    sf::Font      Monaco;
    sf::Text      levelLabel;
    sf::Clock     levelClock;
    bool          showLevelText = false;
    float         levelTextDuration = 2.5;
    Clock  GameOverClk;
    //Text for Volume
    Text VolumeLevel;
    Text VolumeInstructions;
    // Text for High Scores
    sf::Text highScoreText;
    sf::Text highScoreInstruction;
    sf::Text EnterName;
    //Saving game state
    string savedGamePath;
    //Menu tracker
    bool ActivateMenu;
    //Score object
    Scores score;
    // Window
    sf::RenderWindow window;
    sf::Clock playerChange;
    //Time to record gameplay time
    Timer timer;
    //Transitioning stated in game
    bool isRunning;
    bool isPlaying;
    bool isPaused;
    bool isGameOver;
    bool isInMenu;
    bool isInOptions;
    bool isInLevelSelect;
    bool isInNameInput;
    bool isInHighScores;
    bool isInResume;

    // Player name
    std::string playerName;

    bool specialChar = false;
    sf::Clock SpecCharTime;
    float MAX_CHAR_TIME = 15.0f;
    //Menu Pointer
    Menu* menu = nullptr;
    // Private methods
    void initWindow();
    void initTextures();
    void initGameObjects();
    void processEvents();
    void update();
    void render();
    void switchPlayer();
    void renderGameOver();
    void handleGameplayEvents(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                pauseGame();
            }

        }
    }

    void handlePausedEvents(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                resumeGame();
            }
            else if (event.key.code == sf::Keyboard::M) {
                returnToMenu();
            }
        }
    }

    void handleGameOverEvents(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                returnToMenu();
            }
        }
    }

    void handleHighScoreEvents(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                isInHighScores = false;
                isInMenu = true;
            }
        }
    }

    void pauseGame()
    {
        isPlaying = false;
        isPaused = true;
    }

    void resumeGame()
    {
        isPlaying = true;
        isPaused = false;
    }

    void gameOver()
    {
        if (player->GetHp() <= 0)
        {
            std::cout << "Game Over" << std::endl;
            score.saveScore();
        }

        isPlaying = false;
        isGameOver = true;
        GameOverClk.restart();
        renderGameOver();
    }

    void returnToMenu()
    {
        isPlaying = false;
        isPaused = false;
        isGameOver = false;
        isInMenu = true;



    }
    void configureEnemies();
    void cleanupEnemies();
    void advanceToNextLevel();

public:
    Game();
    ~Game();
    void run();
    void showHighScores()
    {
        isInHighScores = true;
        isInMenu = false;
        isPlaying = false;
        isPaused = false;
        isGameOver = false;
        std::string highScores = score.topTwenty();
        highScoreText.setString(highScores);
        //HIGHSCORE AND LABELS
        highScoreText.setFont(Monaco);
        highScoreText.setCharacterSize(35);
        highScoreText.setFillColor(sf::Color::White);
        highScoreText.setOutlineColor(sf::Color::Black);
        highScoreText.setOutlineThickness(2);
        highScoreText.setPosition(100, 100);
        highScoreInstruction.setFont(Monaco);
        highScoreInstruction.setString("Press ESC to return");
        highScoreInstruction.setCharacterSize(50);
        highScoreInstruction.setFillColor(sf::Color::Black);
        highScoreInstruction.setOutlineColor(sf::Color::White);
        highScoreInstruction.setOutlineThickness(2);
        highScoreInstruction.setPosition(70, 30);
    }
    void openOptions()
    {
        isInOptions = true;
        isInMenu = false;
        isPlaying = false;
        isPaused = false;
        isGameOver = false;

        VolumeInstructions.setFont(Monaco);
        VolumeInstructions.setString("Use UP/DOWN arrow keys to adjust volume\nPress ESC to return");
        VolumeInstructions.setCharacterSize(50);
        VolumeInstructions.setFillColor(sf::Color::White);
        VolumeInstructions.setOutlineColor(sf::Color::Black);
        VolumeInstructions.setOutlineThickness(3);
        VolumeInstructions.setPosition(50, 100);

        // Set up VolumeLevel text
        VolumeLevel.setFont(Monaco);
        VolumeLevel.setString("Volume: " + std::to_string(static_cast<int>(AudioSystem::GameVolume)));
        VolumeLevel.setCharacterSize(50);
        VolumeLevel.setFillColor(sf::Color::White);
        VolumeLevel.setOutlineColor(sf::Color::Black);
        VolumeLevel.setOutlineThickness(3);
        VolumeLevel.setPosition(300, 250);

    }
    void handleOptionsEvents(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up)
            {
                audio.increment();
                VolumeLevel.setString("Volume: " + std::to_string(static_cast<int>(AudioSystem::GameVolume)));
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                audio.decrement();
                VolumeLevel.setString("Volume: " + std::to_string(static_cast<int>(AudioSystem::GameVolume)));
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                isInOptions = false;
                isInMenu = true; // Return to main menu
            }
        }
    }



  

    void resume()
    {

        menu->LoadOldState(
            motoBugs, crabs, bats, buzzers, EgStinger,  player, lvl, map->GetLevelWidth(),
            score, coins, diamonds, special
        );

        initTextures();

        initGameObjects();
        timer.start();
        playerChange.restart();
        isInMenu = false;
        isInOptions = false;
        isInNameInput = false;
        isInLevelSelect = false;
        isGameOver = false;
        isPaused = false;
        isPlaying = true;
    }

    void startNewGame(int level)
    {
        isGameOver = false;
        isPaused = false;
        isInMenu = false;
        isInOptions = false;
        isPlaying = true;      
        isInHighScores = false;   
        isInLevelSelect = false; 
        audio.play(level);
        currentLevel = level;
        map = new  Maps(currentLevel);   
		coins = new  RingCoin(*map);
		diamonds = new ExtraLife(*map);
		special = new SpecialBoost(*map);
        lvl = map->getMap();
        initWindow();
        initTextures();
        initGameObjects();
        currentPlayer = 0;
        player = players[currentPlayer];
        player->setOffsetX(0);
        
        player->setPos(100, 300);
        timer.reset();
        timer.start();
        cleanupEnemies();          
        configureEnemies();

        for (int i = 0; i < 4; i++)
        {
            if (currentLevel == 1) {
                players[i]->setFriction(0.8);
                players[i]->setGravity(1);
            }
            else if (currentLevel == 2) {
                players[i]->setFriction(0.92);
                players[i]->setGravity(1);
            }
            else if (currentLevel == 3) {
                players[i]->setFriction(0.85);
                players[i]->setGravity(0.85);
            }
            else if (currentLevel == 4) {
                players[i]->setFriction(0.85);
                players[i]->setGravity(0.95);
            }
        }
       
        levelLabel.setString("Level " + std::to_string(currentLevel) +
            ((currentLevel == 1) ? "\nLabyrinth Zone" :
                (currentLevel == 2) ? "\nIce Cap Zone" :
                "\nDeath Egg Zone"));

        showLevelText = true;
        levelClock.restart();


        if (currentLevel == 1)
        {
            backgroundSprite.setScale(1.8, 1.2);
        }
        else if (currentLevel == 2)
        {
            backgroundSprite.setScale(1, 1);
            wallSprite1.setScale(2, 2);
        }
        else if (currentLevel == 3)
        {
            backgroundSprite.setScale(0.6, 0.5);

        }
        else if (currentLevel == 4)
        {
            backgroundSprite.setScale(0.2, 0.375);


        }

        wallSprite1.setTexture(wallTex1);

        if (currentLevel == 1)
        {
            wallSprite1.setScale(0.63, 0.63);
        }
        else
        {
            wallSprite1.setScale(1, 1);
        }

    }

};



Game::Game() :
    window(sf::VideoMode(screen_x, screen_y), "Sonic the Hedgehog-OOP", sf::Style::Close),
    menu(new Menu(window)),
    timer(180, "Data/Tricky Jimmy.ttf", 10, 2 * 65),
    motoBugs(nullptr), crabs(nullptr), bats(nullptr), buzzers(nullptr),
    EgStinger(nullptr), lvl(nullptr), isInMenu(true), isRunning(true), isPlaying(false), isPaused(false), isGameOver(false),
    score(window),isInOptions(false),  coins(nullptr), diamonds(nullptr), special(nullptr) 
{
    audio.play(0);
    players[0] = new Sonic();
    players[1] = new Knuckles();
    players[2] = new Tails();
    players[3] = new GOKU();
    currentPlayer = 0;
    player = players[currentPlayer];
    float x = player->getX();
    float y = player->getY();
    players[1]->setPos(x - 20, y);
    players[2]->setPos(x - 30, y);
	players[3]->setPos(x, y);
    ActivateMenu = true;
    BatCount = 0;
    MotobugCount = 0;
    CrabCount = 0;
    buzzerCount = 0;
    if (!Monaco.loadFromFile("Data/Tricky Jimmy.ttf"))
        std::cout << "Could not load font\n";
    if (!levelBackgroundTexture.loadFromFile("Data/landscape.jpg"))
        std::cout << "Failed to load Data/landscape.jpg\n";
    levelBackgroundSprite.setTexture(levelBackgroundTexture);
    levelBackgroundSprite.setScale(1.5, 1.25);
    savedGamePath = "save.txt";
}
Game::~Game()
{
    for (int i = 0; i < 4; i++) {
        delete players[i];
    }
    delete menu;
    if (coins) delete coins;
    if (diamonds) delete diamonds;
    if (special) delete special;
    cleanupEnemies();
}

void Game::initWindow()
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(120);
}

void Game::initTextures()
{
    for (int i = 0; i < 4; i++)
    {
        players[i]->setWidth(map->GetLevelWidth());
    }

    //Enemy textures
  
    if (!motoBugTex.loadFromFile("Data/motobug.png"))
        std::cout << "Failed to load Data/motobug.png\n";
    if (!crabTex.loadFromFile("Data/CrabMeat.png"))
        std::cout << "Failed to load Data/CrabMeat.png\n";
    if (!batBrainTex.loadFromFile("Data/BatBrain.png"))
        std::cout << "Failed to load Data/BatBrain.png\n";
    if (!buzzBomberTex.loadFromFile("Data/BuzzBomber.png"))
        std::cout << "Failed to load Data/BuzzBomber.png\n";
    if (!eggStingerTex.loadFromFile("Data/EggStinger.png"))
        std::cout << "Failed to load Data/EggStinger.png\n";
	if (!MeatBallTex.loadFromFile("Data/CrabMeatBall.png"))
		std::cout << "Failed to load Data/CrabMeatBall.png\n";
	if (!Projectile.loadFromFile("Data/egg.png"))
		std::cout << "Failed to load Data/egg.png\n";


    // Load textures
    if (currentLevel == 1)
    {
        if (!backgroundTex.loadFromFile("Data/bg5.png"))
        {
            cout << "Could not load Texture. \n";
        }
        wallTex1.loadFromFile("Data/bl.jpg");
        breakableWallTex.loadFromFile("Data/brick4.png");
    }
    if (currentLevel == 2)
    {
        backgroundTex.loadFromFile("Data/backg12.png");
        wallTex1.loadFromFile("Data/block.png");
        breakableWallTex.loadFromFile("Data/brick8.png");
    }
    if (currentLevel == 3)
    {
        backgroundTex.loadFromFile("Data/BlueSp.jpg");
        wallTex1.loadFromFile("Data/Sp_brick.png");

        breakableWallTex.loadFromFile("Data/brick9.png");
    }
    if (currentLevel == 4)
    {
        backgroundTex.loadFromFile("Data/Boss Bg.jpg");
        wallTex1.loadFromFile("Data/FireBrick.png");
    }

    //Level 1
    greenBushTex.loadFromFile("Data/GreenBush.png");
    brownTowerTex.loadFromFile("Data/BrownTower.png");
    spikeTex.loadFromFile("Data/spike.png");
    //Level 2
    RockTex1.loadFromFile("Data/snowy_rock3.png");
    CrystalTex.loadFromFile("Data/crystal.png");
    JellyTex.loadFromFile("Data/jelly (2).png");
    //Load sprites
    Rock1.setTexture(RockTex1);
    Crystal1.setTexture(CrystalTex);
    Jelly.setTexture(JellyTex);

    //Level 3
    BlackCrystalTex.loadFromFile("Data/crystal_black.png");
    PinkCrystalTex.loadFromFile("Data/crystal_red-pink.png");
    WhiteCrystalTex.loadFromFile("Data/crystal_white-gold.png");
    blueCrystalTex.loadFromFile("Data/crystal_blue.png");
    voiletCrystalTex.loadFromFile("Data/crystal_violet.png");

    // Load sprites
    BlackCrystal.setTexture(BlackCrystalTex), PinkCrystal.setTexture(PinkCrystalTex), WhiteCrystal.setTexture(WhiteCrystalTex), BlueCrystal.setTexture(blueCrystalTex), voiletCrystal.setTexture(voiletCrystalTex);
    // Set up sprites
    backgroundSprite.setTexture(backgroundTex);
    if (currentLevel == 1)
    {
        backgroundSprite.setScale(1.8, 1.2);
    }
    else if (currentLevel == 2)
    {
        backgroundSprite.setScale(1, 1);
        wallSprite1.setScale(2, 2);
    }
    else if (currentLevel == 3)
    {
       backgroundSprite.setScale(0.6, 0.5);
     
    }
    else if (currentLevel==4)
    {
        backgroundSprite.setScale(0.2, 0.375);
        
      
    }

    wallSprite1.setTexture(wallTex1);

    if (currentLevel == 1)
    {
        wallSprite1.setScale(0.63, 0.63);
    }
    else
    {
        wallSprite1.setScale(1, 1);
    }
    //Board
	CompleteLevelBoardTex.loadFromFile("Data/GameEnd.png");
	CompleteLevelBoardSprite.setTexture(CompleteLevelBoardTex);

    breakableWallSprite.setTexture(breakableWallTex);
    greenBushSprite.setTexture(greenBushTex);
    brownTowerSprite.setTexture(brownTowerTex);
    spikeSprite.setTexture(spikeTex);

    if (!Monaco.loadFromFile("Data/Tricky Jimmy.ttf"))
        std::cout << "Could not load font\n";

    levelLabel.setFont(Monaco);
    levelLabel.setCharacterSize(100);
    levelLabel.setOutlineThickness(5);
    levelLabel.setOutlineColor(sf::Color::Black);
    levelLabel.setFillColor(sf::Color::White);
    levelLabel.setPosition(450, 300);




}

void Game::initGameObjects() 
{
    int thislvlWidth = map->GetLevelWidth();
    std::cout << thislvlWidth << std::endl;
    //for (int i = 0; i < 4; i++) {
    //    players[i]->setWidth(thislvlWidth); // Set width for Sonic, Knuckles, Tails
    //}
    lvl = map->getMap();
    coins->place();
    diamonds->place();
    special->place();
    playerChange.restart();
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            isRunning = false;
        }

        if (isInMenu)
        {
           
         
            menu->handleEvent(event);
            if (menu->isEnterPressed())
            {
                audio.play(0);
               
                int choice = menu->getSelectedIndex();
                menu->resetEnter();

                switch (choice)
                {
                case 0:
                {   // New Game
                    isInMenu = false;
                    isInNameInput = true;
                    menu->enterName();

                    break;
                }
                case 1:     // Resume
                    resume();    isInMenu = false;  break;
                case 2:     // Options
                    openOptions();
                     break;
                case 3:     // High Scores
                    showHighScores(); break;
                case 4:     // Exit
                    window.close();  break;
                }
            }

        
        }
       
        if (event.type == sf::Event::KeyPressed
            && event.key.code == sf::Keyboard::S
            && event.key.control)
        {
            menu->saveCurrentState(motoBugs, crabs, bats, buzzers, EgStinger, player, lvl, map->GetLevelWidth(), score);
        }
        if (isInNameInput)
        {
            std::cout << "In Name Input State\n";
            menu->handleNameInput(event);
            if (!menu->isNameEntered()) 
            {
                std::cout << "Name Entered: " << menu->GetPlayerName() << "\n";
                playerName = menu->GetPlayerName();
                score.addName(playerName);
                isInNameInput = false;
                isInLevelSelect = true;
            }
        }
        else if (isInLevelSelect)
        {
            menu->handleEvent(event);
            int level = menu->selectLevel();
            if (level > 0) {
                isInLevelSelect = false;
                startNewGame(level);
                isPlaying = true;
            }
            else if (level == 0) {
                isInLevelSelect = false;
                isInMenu = true;
            }
        }

        else if (isPlaying) 
        {
            handleGameplayEvents(event);
        }
        else if(isInOptions)
        {
            handleOptionsEvents(event);
        }
        else if (isPaused)
        {
            handlePausedEvents(event);
        }
        else if (isGameOver)
        {
            handleGameOverEvents(event);
        }
        else if(isInNameInput)
        {
            
            menu->handleNameInput(event);
        }
        if (isInHighScores) 
        {
            handleHighScoreEvents(event);
        }
       
    }
}

void Game::update()
{

    if (player->GetHp() <= 0)
    {
        std::cout << "Game Over" << std::endl;
        gameOver();
    }

    if (showLevelText)
    {
        float t = levelClock.getElapsedTime().asSeconds();
        if (t >= levelTextDuration)
        {
            showLevelText = false;
        }
        else
        {
          
            levelLabel.setFillColor(sf::Color::White);

        }
        return;
       
    }

    if (!player || !lvl) return;
    // Update player
    bool BOSSLEVEL = currentLevel == 4;
    bool ismoving = player->movement(lvl, true, BOSSLEVEL);
    //std::cout << player->getOffsetX() << " " << player->getX() << " " <<player->getOffsetY()<< std::endl;
    bool isflying = player->getIsFlying();
   
    player->punching(lvl, true);
    player->player_gravity(lvl);
    player->update();

    for (int i = 0; i < 4; i++)
    {
        if (currentPlayer == 0) {
            players[i]->setSpeed(15);
        }
        else if (currentPlayer == 1) {
            players[i]->setSpeed(10);
        }
        else if (currentPlayer == 2) {
            players[i]->setSpeed(8);
        }
        if (i != currentPlayer) {
            players[i]->setOffsetX(player->getOffsetX());
            if (player->getDirection()) {
                players[i]->setX(player->getX() + 40);
            }
            else {
                players[i]->setX(player->getX() - 40);
            }
            if (isflying) {
                players[i]->hang(player->getX(), player->getY(), player->getOffsetX());
            }
            else {
                players[i]->movement(lvl, false, false);
            }
            players[i]->punching(lvl, false);
            players[i]->player_gravity(lvl);
            players[i]->update();

        }
    }
    timer.update();
    if (!motoBugs || !buzzers || !bats || !crabs) 
    {
        return;
    }
    // Update enemies
    for (int i = 0; i < MotobugCount; ++i) {
        motoBugs[i].move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);
        // Check collisions with enemies
        int dmg = motoBugs[i].giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score, player->getPower());
        if (dmg > 0) player->takeDamage(dmg);
    }
    for (int i = 0; i < BatCount; i++)
    {
        bats[i].move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);     
        int dmg = bats[i].giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score, player->getPower());
        if (dmg > 0) player->takeDamage(dmg);
    }
    for (int i = 0; i < buzzerCount; i++)
    {
        buzzers[i].move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);
        int dmg = buzzers[i].giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score, player->getPower());
        if (dmg > 0) player->takeDamage(dmg);
    }
    for (int i = 0; i < CrabCount; i++)
    {
        crabs[i].move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);
        int dmg = crabs[i].giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score, player->getPower());
        if (dmg > 0) player->takeDamage(dmg);
    }
    for (int i = 0; i < EggStCount; i++)
    {
        EgStinger[i].move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);
        int dmg = EgStinger[i].giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score, player->getPower());
        if (dmg > 0) player->takeDamage(dmg);
    }

    // Check collectable collisions
    coins->checkCollision(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), player->gethitX(), player->gethitY(), score, *player);
    diamonds->checkCollision(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), player->gethitX(), player->gethitY(), score, *player);
    special->checkCollision(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), player->gethitX(), player->gethitY(), score, *player);

    // Animate collectables
    coins->animate();
    diamonds->animate();
    special->animate();
    ActivateMenu = true;

    //Check if end reached
    float centerX = player->getX() + 40 / 2;
    float centerY = player->getY() + 40 / 2;
    int ti = centerY / cell_size;
    int tj = (centerX + player->getOffsetX()) / cell_size;

    if (ti >= 0 && ti < 14 && tj >= 0 && tj < map->GetLevelWidth() && lvl[ti][tj] == 'E') {
        advanceToNextLevel();
    }



}

void Game::render()
{
    window.clear(sf::Color::Black);
    if (showLevelText)
    {
        window.draw(levelBackgroundSprite);
        window.draw(levelLabel);
        window.display();
        return;
    }

    if (isInMenu)
    {
        menu->draw(window);
        menu->PlayMenuMusic();
    }

    if (isInOptions)
    {
        
    
        window.draw(levelBackgroundSprite); 
        window.draw(VolumeInstructions);
        window.draw(VolumeLevel);
        window.display();
        return;
    }
    if (isInNameInput) 
    {
        menu->drawNameInput(window);
        window.display();
        return;
    }
    if (isInHighScores)
    {
        window.draw(levelBackgroundSprite);
        window.draw(highScoreText);
        window.draw(highScoreInstruction);
        window.display();
        return;
    }

   
    // Draw background
    backgroundSprite.setPosition(-player->getOffsetX() / 7, 0);
    window.draw(backgroundSprite);

    
    // Draw level
    for (int i = 0; i < height; i += 1) {
        for (int j = player->getOffsetX() / 64; j < (1300 + player->getOffsetX()) / 64; j += 1) {
            if (lvl[i][j] == 'w')
            {
                //wallSprite2.setScale(2, 2);
                wallSprite1.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(wallSprite1);
            }


            if (lvl[i][j] == 'B')
            {
                greenBushSprite.setScale(2, 2);
                greenBushSprite.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(greenBushSprite);
            }
            if (lvl[i][j] == 'T')
            {
                brownTowerSprite.setScale(4, 4);
                brownTowerSprite.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(brownTowerSprite);
            }
            if (lvl[i][j] == 'k')
            {
                spikeSprite.setScale(1.5, 1.5);
                spikeSprite.setPosition(j * cell_size - player->getOffsetX(), i * cell_size - 10);
                window.draw(spikeSprite);
            }
            if (lvl[i][j] == 'b')
            {
                breakableWallSprite.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(breakableWallSprite);
            }

            if (lvl[i][j] == 'R')
            {
                Rock1.setScale(1.25, 1.25);

                Rock1.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(Rock1);
            }
            if (lvl[i][j] == 'J')
            {
                Jelly.setScale(1, 1);
                Jelly.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(Jelly);
            }



            if (lvl[i][j] == 'C')
            {
                Crystal1.setScale(1.25, 1.25);
                Crystal1.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(Crystal1);
            }
            if (lvl[i][j] == 'K')
            {
                BlackCrystal.setScale(1, 1);
                BlackCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(BlackCrystal);
            }
            if (lvl[i][j] == 'P')
            {
                PinkCrystal.setScale(1, 1);
                PinkCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(PinkCrystal);
            }
            if (lvl[i][j] == 'W')
            {
                WhiteCrystal.setScale(1, 1);
                WhiteCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(WhiteCrystal);
            }
            if (lvl[i][j] == 'U')
            {
                BlueCrystal.setScale(1, 1);
                BlueCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(BlueCrystal);
            }
            if (lvl[i][j] == 'V')
            {
                voiletCrystal.setScale(1, 1);
                voiletCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(voiletCrystal);
            }
            if (lvl[i][j]=='E')
            {
                CompleteLevelBoardSprite.setScale(1.5, 1.5);
                CompleteLevelBoardSprite.setPosition(j* cell_size - player->getOffsetX(), i* cell_size);
                window.draw(CompleteLevelBoardSprite);
            }
        }
    }

    // Draw enemies
    for (int i = 0; i < MotobugCount; ++i) {
        motoBugs[i].draw(window);
        motoBugs[i].animateSprite();
    }
	for (int i = 0; i < BatCount; ++i) {
		bats[i].draw(window);
		bats[i].animateSprite();
	}

    for (int i = 0; i < buzzerCount; i++)
    {
        buzzers[i].draw(window);
		buzzers[i].animateSprite();
    }

    for (int i = 0; i < CrabCount; i++)
    {
        crabs[i].draw(window);
		crabs[i].animateSprite();
    }
    for (int i = 0; i < EggStCount; i++)
    {
        EgStinger[i].draw(window);
        EgStinger[i].animateSprite();
    }

    // Draw player
    //player->draw_player(window);
    for (int i = 0; i < 3; i++) {
        if (i != currentPlayer && !specialChar) {
            players[i]->draw_player(window);
        }
    }
    player->draw_player(window);

    // Draw collectables
    coins->draw(window, player->getOffsetX());
    diamonds->draw(window, player->getOffsetX());
    special->draw(window, player->getOffsetX());
    score.DisplayScoreWin(*player);
	timer.draw(window);
    window.display();
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();

        if (isInMenu)
        {
           
            window.clear();
            menu->draw(window);
            window.display();
            
        }
        else if (isGameOver)
        {
            renderGameOver();
        }
        else if (isInOptions)
        {
            render(); 
        }
        else if (isPlaying)
        {
            update();
            render();    // only after we’ve set up everything
            switchPlayer();
        }
        else if(isInNameInput)
        {
            render();
        }
        else if (isInHighScores) {
            render();   

        }
        if (isInResume) 
        {
            isInResume = false;
            isPlaying = true;
            continue;
        }

    }
}



void Game::switchPlayer()
{
    // Static variables to track OOP sequence detection
    static int sequenceState = 0;
    static bool wasOPressed = false;
    static bool wasPPressed = false;

    if (SpecCharTime.getElapsedTime().asSeconds() >= 15 && specialChar) {
        SpecCharTime.restart();
		specialChar = false;
		currentPlayer = 0;
        for (int i = 0; i < 4; i++) {
			players[i]->setPos(player->getX(), player->getY());
			players[i]->setOffsetX(player->getOffsetX());
			players[i]->setVelocityY(player->getVelocityY());
        }
		player = players[currentPlayer];
    }
    // Check for regular player switch with C key
    if (Keyboard::isKeyPressed(Keyboard::Z) && !specialChar)
    {
        if (playerChange.getElapsedTime().asSeconds() >= 1.5f) {
            float x = player->getX();
            float y = player->getY();
            float velocityY = player->getVelocityY();
            float offsetX = player->getOffsetX();
            // Switch to next character
            currentPlayer = (currentPlayer + 1) % 3;
            player = players[currentPlayer];
            // Apply position and state to new character
            player->setPos(x, y);
            player->setOffsetX(offsetX);
            // Optional: Apply velocity to maintain momentum
            player->setVelocityY(velocityY);
            playerChange.restart();
        }
    }

    // Check for O key press in OOP sequence
    if (Keyboard::isKeyPressed(Keyboard::O))
    {
        if (!wasOPressed) {
            wasOPressed = true;

            // Update sequence state
            if (sequenceState == 0 || sequenceState == 1) {
                sequenceState++;
            }
            else {
                sequenceState = 1; // Reset to first 'O'
            }
        }
    }
    else {
        wasOPressed = false;
    }

    // Check for P key press in OOP sequence
    if (Keyboard::isKeyPressed(Keyboard::P) && SpecCharTime.getElapsedTime().asSeconds() >= 5)
    {
        if (!wasPPressed) {
            wasPPressed = true;

            // Update sequence state
            if (sequenceState == 2) {
                sequenceState = 0; // Reset after completing sequence

                std::cout << " OOP sequence detected! " << std::endl;
                specialChar = true;
                SpecCharTime.restart();
                float x = player->getX();
                float y = player->getY();
                float velocityY = player->getVelocityY();
                float offsetX = player->getOffsetX();
				float offsetY = player->getOffsetY();
                currentPlayer = 3;
                player = players[currentPlayer];
                // Apply position and state to new character
                player->setPos(x, y);
                player->setOffsetX(offsetX);
                player->setOffsetY(offsetY);
                // Optional: Apply velocity to maintain momentum
                player->setVelocityY(velocityY);
                if (currentLevel == 4) {
					int hp = EgStinger->getHP();
                    EgStinger->setHP(hp / 2);
                }
            }
            else {
                sequenceState = 0; // Reset on wrong key
            }
        }
    }
    else {
        wasPPressed = false;
    }
}
void Game::renderGameOver()
{
   
    
    if (GameOverClk.getElapsedTime().asSeconds() >= 1.0)
    {
        isGameOver = false;
        isInMenu = true; 
    }
    else
    {
        // Draw game over text
        sf::Font font;
        if (font.loadFromFile("Data/Tricky Jimmy.ttf")) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER");
            gameOverText.setOutlineThickness(4);
            gameOverText.setOutlineColor(sf::Color::Black);
            gameOverText.setCharacterSize(120);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setPosition(350, 300);
            window.draw(gameOverText);
            window.display();
        }
        else 
        {
            std::cout << "Failed to load font for Game Over screen: Data/Tricky Jimmy.ttf" << std::endl;
        }
       
    }
}
void Game::advanceToNextLevel() {
  //sAVE SCORE AND SHI
    int next = currentLevel + 1;
    if (next >= MAX_LEVELS)
    {

        gameOver();
    }
    else 
    {
        score.addLevelClear();
        score.saveScore();
        startNewGame(next);
        isInMenu = false;
        isPlaying = true;
    }
}


void Game::configureEnemies()  
{  
   // 1) first delete any old arrays (if reconfiguring mid-game)  
   cleanupEnemies();  

   // 2) pick counts for this level  
   switch (currentLevel)  
   {  
   case 1:  
       CrabCount = 1;  
       BatCount = 2;  
       MotobugCount = 2;  
       buzzerCount = 1;  
       EggStCount = 0;
       break;  
   case 2:  
       CrabCount = 2;  
       BatCount = 2;  
       MotobugCount = 2;  
       buzzerCount = 2; 
       EggStCount = 0;
       break;  
   case 3:  
       CrabCount = 3;  
       BatCount = 4;  
       MotobugCount = 4;  
       buzzerCount = 3;  
       EggStCount = 0;
       break;  
   default:  
       CrabCount = 0;  
       BatCount = 0;  
       MotobugCount = 0;  
       buzzerCount = 0; 
       EggStCount = 1;

   }  

   // 3) allocate new arrays  
   crabs = new CrabMeat[CrabCount];  
   bats = new BatBrain[BatCount];  
   motoBugs = new MotoBug[MotobugCount];  
   buzzers = new BuzzBomber[buzzerCount];  
   EgStinger = new EggStinger[EggStCount];


   // 4) (optional) initialize each with its spawn point or map reference  
   for (int i = 0; i < CrabCount; ++i)
   {
       if (i == 0 && currentLevel == 2)
       {
           crabs[i] = CrabMeat(5500 * (i + 1), 350, map->getMap());
       }
       else if (i == 1) 
       {
           crabs[i] = CrabMeat(4500 * (i + 1), 750, map->getMap());
           if (currentLevel == 3) 
           {
               crabs[i] = CrabMeat(4500 * (i + 1), 490, map->getMap());
           }
       }
       else  if (i == 2) 
       {
           crabs[i] = CrabMeat(5500 * (i + 1), 480, map->getMap());
       }
       else
       crabs[i] = CrabMeat(5500 * (i + 1), 400, map->getMap());
       crabs[i].getEnemySprite().setTexture(crabTex);
       crabs[i].getMeatBall().setTexture(MeatBallTex);
   }
   for (int i = 0; i < BatCount; ++i)
   {
	   if (i == 1)
	   {
		   bats[i] = BatBrain(5200 * (i + 1), 300, map->getMap());
	   }
	   else
       bats[i] = BatBrain(700 * (i + 1), 300, map->getMap()); 
	   bats[i].getEnemySprite().setTexture(batBrainTex);
   }
   for (int i = 0; i < MotobugCount; ++i)
   {
       if (i == 1)
       {
           motoBugs[i] = MotoBug(4000 * (i + 1), 730, map->getMap());
           if (currentLevel == 3) 
           {
               motoBugs[i] = MotoBug(3600 * (i + 1), 490, map->getMap());
           }
       }
       
       else
       motoBugs[i] = MotoBug(3500 * (i + 1), 730, map->getMap());
       motoBugs[i].getEnemySprite().setTexture(motoBugTex);
   }
   for (int i = 0; i < buzzerCount; ++i)
   {
       if (i == 1 && currentLevel == 2) 
       {
           buzzers[i] = BuzzBomber(10000 * (i + 1), 100, map->getMap());
       }
       buzzers[i] = BuzzBomber(7000 * (i + 1), 100, map->getMap());
       buzzers[i].getEnemySprite().setTexture(buzzBomberTex);
       buzzers[i].getProjSprite().setTexture(Projectile);
   }
   for (int i = 0; i < EggStCount; i++)
   {
       EgStinger[i] = EggStinger(500, 300, map->getMap());
       EgStinger[i].GetEnemySprite().setTexture(eggStingerTex);
   }
}

void Game::cleanupEnemies()
{
    if (crabs) {
        delete[] crabs;
        crabs = nullptr;
        CrabCount = 0;
    }
    if (bats) {
        delete[] bats;
        bats = nullptr;
        BatCount = 0;
    }
    if (motoBugs) {
        delete[] motoBugs;
        motoBugs = nullptr;
        MotobugCount = 0;
    }
    if (buzzers) {
        delete[] buzzers;
        buzzers = nullptr;
        buzzerCount = 0;

    }
    if (EgStinger) 
    {
        delete[] EgStinger;
        EgStinger = nullptr;
        EggStCount = 0;
    }
}

