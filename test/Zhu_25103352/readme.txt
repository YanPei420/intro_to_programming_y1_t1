Robot Rescue Mission
====================

Student Name:
-------------
Yan Pei Zhu

Student Number:
---------------
25103352

Short description
-----------------
Ncurses terminal game where a robot must rescue a person while avoiding mines, walls and a cross-shaped obstacle.

Requirements
------------
- Ubuntu in VMBox using libraries such as ncurses

Build and run
-------------
-gcc game.c -o game.exe -lncurses && ./game.exe

Controls
--------
- Arrow keys: move robot (manual mode)
- When starting the game, press arrow keys for manual mode and press 'm' for AI mode
- On-screen prompts appear for menus and leaderboard

Gameplay rules
--------------
- Player starts with 3 lives.
- Loses 1 life on collision with wall, obstacle, or mine.
- Rescue the person (reach 'P') to earn +1 score.
- Every 5 points -> level up:
  - spawn +2 mines
  - increase robot speed by 2 (speed units used by game)
- Only the robot body counts for collisions and rescues, the head is only there to indicate direction
- press m to switch between AI mode and manual mode

Files
-----
- game.c                (main game code)
- README.txt            (this file)
- leaderboard.txt       (scores saved here)

Notes
-----
-There's a lot of commented code in the game.c such as the function power ups, please just ignore these functions and other commented code that aren't used