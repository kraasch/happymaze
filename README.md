# HappyMaze

This is a copy of a game I played as a kid in the 90s, where you could play a
smiley in a maze, hunting down other players and shooting them with a ball.

The interface is terminal-based and pseudo 3D (similar to the first Wolfenstein game).
The original was a LAN multiplayer game, where one wins by hunting down other
players in a maze and shooting them with a ball. It might have been the 1987
game [MIDI Maze](https://en.wikipedia.org/wiki/MIDI_Maze), but my memory is blurred. (Also checkout [MidiMaze2](https://github.com/sarnau/MidiMaze2)).

## Demo and notes

Compile and run with: `g++ HappyMaze.cpp -o HappyMaze -lncurses && ./HappyMaze`.

The first person perspective into the maze.

![IMAGE DEMO](/info/demo.png)

## Tasks

   - [ ] re-write in Rust (?).
   - [ ] sidewards steps .
   - [ ] collision detection (hit walls) .
   - [ ] network multiplayer (client-side hosted).
   - [ ] shooting .
   - [ ] automated maze generation .
   - [X] vertical lines in walls for orientation .

## Meta

Sources:

   - https://github.com/OneLoneCoder/CommandLineFPS .
   - https://github.com/sarnau/MidiMaze2 .
   - https://stackoverflow.com/questions/7317523/what-should-i-use-instead-of-windows-h-in-linux .
   - http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/ .

Further reading:

   - http://www.midimaze.de/ .
   - http://www.esglabs.com/midimaze/ .
   - https://en.wikipedia.org/wiki/Wolfenstein_3D .
   - https://en.wikipedia.org/wiki/MIDI_Maze .
   - https://en.wikipedia.org/wiki/List_of_maze_video_games .
   - https://en.wikipedia.org/wiki/Jinn .
   - http://i3.kym-cdn.com/photos/images/newsfeed/000/288/648/776.gif 
   - https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c#1727896 .
   - https://youtu.be/xW8skO7MFYw (Code-It-Yourself! First Person Shooter, Quick and Simple C++) .
   - https://youtu.be/BSb87DC-PtA (RetroAhoy: Wolfenstein 3D) .
