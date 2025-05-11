#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

class AudioSystem {
private:
    sf::Music* musics;    // array of Music objects
    int        trackCount;
    int        current;   // which track is playing now

public:
    static int GameVolume;
    AudioSystem(int numTracks = 5)
    {
        current = 0;
        musics = new sf::Music[5];
        trackCount = 5;

        //MenuScreen
        if (!musics[0].openFromFile("Data/01-Title Screen.ogg"))
        {
            std::cout << "Could not load file 01-Title Screen.ogg " << std::endl;
        }
        //Level:1
        if (!musics[1].openFromFile("Data/labrynth.ogg"))
        {
            std::cout << "Could not load file labrynth.ogg " << std::endl;
        }
        //Level:2
        if (!musics[2].openFromFile("Data/04-Marble-Zone-Act-1.ogg"))
        {
            std::cout << "Could not load file 04-Marble-Zone-Act-1.ogg " << std::endl;

        }
        //Level:3
        if (!musics[3].openFromFile("Data/06-Starlight-Zone.ogg"))
        {
            std::cout << "Could not load file 06-Starlight-Zone.ogg " << std::endl;

        }
        //Level:4(Boss)
        if (!musics[4].openFromFile("Data/08-Boss.ogg"))
        {
            std::cout << "Could not load file 08-Boss.ogg " << std::endl;

        }
    }



        void play(int idx)
        {
            if (idx < 0 || idx >= trackCount) return;
            // if something already playing, stop it
            if (current >= 0 && musics[current].getStatus() == sf::Music::Playing)
                musics[current].stop();

            current = idx;
            musics[current].setVolume(GameVolume);
            musics[current].setLoop(true);
            musics[current].play();
        }

        void stop()
        {
            if (current >= 0 && musics[current].getStatus() == sf::Music::Playing)
                musics[current].stop();
            current = -1;
        }

        void pause()
        {
            if (current >= 0 && musics[current].getStatus() == sf::Music::Playing)
                musics[current].pause();
        }

        void resume()
        {
            if (current >= 0 && musics[current].getStatus() == sf::Music::Paused)
                musics[current].play();
        }

        void setLoop(int idx, bool loop)
        {
            if (idx < 0 || idx >= trackCount) return;
            musics[idx].setLoop(loop);
        }

        void setVolume(int idx, float volume)
        {
            if (idx < 0 || idx >= trackCount) return;
            musics[idx].setVolume(volume);
        }
        void increment() 
        {
            if (GameVolume < 100) 
            {
                GameVolume++;
            }
            else
            {
                return;
            }
        }
        void decrement()
        {
            if (GameVolume> 0)
            {
                GameVolume--;
            }
            else
            {
                return;
            }
        }

        ~AudioSystem()

        {

            stop();
            delete[] musics;
        }



};
int AudioSystem::GameVolume = 5;




