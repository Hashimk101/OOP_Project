// Timer.h
#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


class Timer {
private:
    sf::Clock  clock;         
    float      timeLimit;     
    bool       isRunning;     
    float      elapsed;    

    // graphics
    sf::Font   font;
    sf::Text   TimerText;
    float pos_x, pos_y;

public:
   
    Timer(float limit, const std::string& fontFile, float x, float y);

    ~Timer() = default;

    // time control
    void start();
    void stop();
    void reset();
    void update();             

    // queries
    bool isTimeUp()     const;
    float getRemainingTime() const;
    float getcurrTime();

    // graphics
    void draw(sf::RenderWindow& window);
};

Timer::Timer(float limit, const std::string& fontFile, float x, float y) : timeLimit(limit),isRunning(false), elapsed(0.f),  pos_x(x), pos_y(y)
{
    if (!font.loadFromFile(fontFile))
        throw std::runtime_error("Failed loading font for Timer");

    TimerText.setFont(font);
    TimerText.setCharacterSize(50);
    TimerText.setFillColor(sf::Color::White);
    TimerText.setOutlineColor(sf::Color::Black);
    TimerText.setOutlineThickness(7.f);
    TimerText.setPosition(pos_x, pos_y);
}

void Timer::start() {
    clock.restart();
    isRunning = true;
}

void Timer::stop() {
    if (isRunning) {
        elapsed = clock.getElapsedTime().asSeconds();
        isRunning = false;
    }
}

void Timer::reset() {
    clock.restart();
    elapsed = 0.f;
    isRunning = false;
}

void Timer::update() {
    if (isRunning)
    {
        elapsed = clock.getElapsedTime().asSeconds();
    }
}

bool Timer::isTimeUp() const {
    return elapsed >= timeLimit;
}

float Timer::getRemainingTime() const
{
    float rem = timeLimit - elapsed;
    return (rem > 0.f ? rem : 0.f);
}

void Timer::draw(sf::RenderWindow& window) {
    // Format as MM:SS
    float rem = getRemainingTime();
    int   m = int(rem) / 60;
    int   s = int(rem) % 60;

    std::string sec = (s < 10 ? "0" : "") + std::to_string(s);
    TimerText.setString("Timer: "+ std::to_string(m) + ":" + sec);


    window.draw(TimerText);
}

float Timer::getcurrTime() 
{
	return elapsed;
}