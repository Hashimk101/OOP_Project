#pragma once  
#include <iostream>  
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std; 


class Menu  
{  private:
	sf::Font Monaco;
	sf::Texture BackgroundTex;
	sf::Sprite BackgroundSprite;
	sf::RenderWindow& window;
	sf::Text* text;
	sf::Text Title;
	String* options;
	string TlTxt;
	int SelectedIndex;
	const int numOptions = 5;
	bool keyHandled = false;
	sf::Color lightBlue;
	Texture BannerTxture;;
	sf::Sprite* OptionBanners;
public:
	Menu(RenderWindow& GameWindow) : numOptions(5), window(GameWindow), lightBlue(180, 236, 250)
	{
		if (!Monaco.loadFromFile("Data/Tricky Jimmy.ttf")) {
			std::cout << "Error loading font file: Data/Monaco.ttf" << std::endl;

		}
		if (!BackgroundTex.loadFromFile("Data/menubg.png")) {
			std::cout << "Error loading background texture: Data/bg5.png" << std::endl;
		}
		if (!BannerTxture.loadFromFile("Data/IDK.png")) {
			std::cout << "Error loading banner texture: Data/IDK.png" << std::endl;
		}
		BackgroundSprite.setTexture(BackgroundTex);
		options = new String[5];
		options[0] = "New Game";
		options[1] = "Resume";
		options[2] = "Options";
		options[3] = "High Scores";
		options[4] = "Exit";

		text = new Text[5];
		for (int i = 0; i < 5; i++)
		{
			text[i].setFont(Monaco);
			text[i].setString(options[i]);
			text[i].setCharacterSize(60);
			text[i].setFillColor(sf::Color::White);
			text[i].setPosition(450, 325 + i * 150);
			
		}
		OptionBanners = new sf::Sprite[numOptions];
		for (int i = 0; i < numOptions; i++)
		{
			OptionBanners[i].setTexture(BannerTxture);
			OptionBanners[i].setScale(0.75, 0.75); // adjust as needed
			OptionBanners[i].setPosition(650, 300 + i * 150); 
			OptionBanners[i].setOrigin(385, 50);
		}
		SelectedIndex = 0;
		text[SelectedIndex].setFillColor(lightBlue);
		TlTxt = "Sonic Classic Heroes";
		Title.setCharacterSize(120);
		Title.setFont(Monaco);
		Title.setFillColor(sf::Color::White);
		Title.setPosition(105, 100);
		Title.setString(TlTxt);
		keyHandled = false;
		BackgroundSprite.setScale(2, 2.25);
	}
	void draw(RenderWindow& GameWindow)
	{
		
		GameWindow.draw(BackgroundSprite);
		GameWindow.draw(Title);
		for (int i = 0; i < 5; i++)
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
			text[SelectedIndex].setFillColor(lightBlue);
		}
		else
		{
			SelectedIndex--;
			text[SelectedIndex].setFillColor(lightBlue);

		}
	}
	void moveDown() 
	{
		text[SelectedIndex].setFillColor(sf::Color::White);
		if (SelectedIndex == numOptions - 1)
		{
			SelectedIndex = 0;
			text[SelectedIndex].setFillColor(lightBlue);
		}
		else
		{
			SelectedIndex++;
			text[SelectedIndex].setFillColor(lightBlue);
		}
	}
	void handleEvent(const sf::Event& event) {
		if (event.type == sf::Event::KeyPressed && !keyHandled) {
			switch (event.key.code) {
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
					std::cout << "Starting a new game..." << std::endl;
					// Add logic to start a new game
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
	}


};