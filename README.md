# 🎮 SONIC CLASSIC HEROS
> A recreation of SEGA's classic Sonic, crafted with C++17 and SFML 2.6.1, featuring procedural maps, animated sprites, collectables, basic AI enemies, and a Heads Up Display (timer + scoreboard)
## 🚀 Table of Contents  
1. [About](#about)  
2. [Features](#features)  
3. [Working (Architecture & Flow)](#working-architecture--flow)  
4. [Game Controls](#game-controls)  
5. [Project Structure](#project-structure)  
6. [Contributing](#contributing)  
7. [Contact](#contact)  

## 📖 About  
Get ready to dash through a nostalgic side-scrolling platformer inspired by SEGA’s classic Sonic! In Sonic Classic Heroes, guide Sonic and friends through procedurally generated levels brimming with loops, ramps, and secrets. Collect crystals, outsmart pesky enemies, and race the clock to claim the top spot on the leaderboard. 
## 🔥Features
>Procedural Map Loader 📍: Levels generated from text-file grids, creating unique tile-based worlds every run.<br>
>Sprite Animation 🎥: Smooth moves for Sonic & co.—walk, jump, fly, and punch with pixel-perfect flair.<br>
>Collectables: coins for points🪙, crystals for health 💎, and smash breakable walls  for shortcuts.<br>
> Levels🗺️:<br>
1. Labyrinth Zone: Easy, 200×14 cells, standard friction.<br>
2. Ice Cap Zone: Medium, 250×14 cells, low friction (higher acceleration/deceleration).<br>
3. Death Egg Zone: Hard, 300×14 cells, low gravity, reduced player speeds.<br>
4. Boss Level: Face Egg Stinger in a high-stakes showdown.<br>
>Enemies 🤖: Clever  enemies spawn off-screen, patrol to waypoints, and chase when you get too close!<br>
>HUD ⏱️: Retro-styled display tracks your score, a countdown timer, and persistent high scores across sessions.<br>
>Audio 🎵: Pump up the jams with level-specific background music.<br>
>Menu & Pause 🖱️: Navigate slick menus, pause/resume gameplay, or restart levels with ease.<br>
>Classic Sonic Vibes 🌀: Authentic physics for spin-dashes, loops, and high-speed action.<br>

## ⚙️working-architecture--flow
Entry Point (Source.cpp) <br>
Creates a Game object and kicks off the fun with game.run().<br>
Game Loop (Game::run())<br>
<b>⚡Game Flow
1. Start with the menu (New Game, Options,  Leaderboard).
2. Play through 3 levels (collect all rings, reach the end) and the boss level (defeat Egg Stinger).
3. Lose if HP = 0 or timer expires; win by completing all levels.
 ## 🎮 Game Controls
* Arrow Keys: Move left/right, jump and spin.
* T: Punch (Knuckles breaks walls).
* Z: Switch between Sonic, Tails, and Knuckles.
* F: Flying of Tails

## 📂 Project Structure
```plaintext
sonic-classic-heroes
├──Data/               # Sprites, audio, level files
│   ├── sprites         # Sprite sheets (Sonic, Tails, Knuckles, enemies)
│   ├── audio         # BGM, SFX (jump, collect, damage)
│   └── levels          # Text files for procedural maps (14×200 grids)
├── src               # Source code
├── assets               # Sprites, audio, level files
│   ├── sprites          # Sprite sheets (Sonic, Tails, Knuckles, enemies)
│   ├── audio            # BGM, SFX (jump, collect, damage)
│   └── levels           # Text files for procedural maps (14×200 grids)
├── src                   # Source code
│   ├── Source.cpp          # Entry point
│   ├── Game.h            # Game loop, core logic
│   ├──Header.h          # Player mechanics (movement, abilities)
│   ├── Enemy.h          # Enemy AI (Bat Brain, Bee-Bot, etc.)
│   ├── Map.h            # Procedural map loader
│   ├── Menu.h           # Menu Integration
│   ├── Collectables.h   # Implementation of collectables(Collision detection and animation)
│   ├──  Timer.h        # Tracks gameplay time
│   ├──  Score Board.h   # Tracks high score
│   ├── Audio.h          # Controls the music throughout the game
├── README.md             # This file
└── CONTRIBUTING.md       # Contribution guidelines
├── README.md             # This file
└── CONTRIBUTING.md       # Contribution guidelines

