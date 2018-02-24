
// c libraries .
#include <ncurses.h>
#include <unistd.h>
#include <math.h>

// c++ libraries .
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// random numbers .
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// constants .
float DEPTHMAX = 16.0f;

// global variables .
float playerX = 2.0f;
float playerY = 2.0f;
float playerDir = 0.0f;
int mapHeight = 16;
int mapWidth  = 16;
float fov = M_PI / 4.0f; // Field Of View .

int main(int argc, char **argv) {

  // setup game data.
  wstring map;
  map += L"################"; 
  map += L"#..#..#...#....#";
  map += L"#..#.....#.....#";
  map += L"#..#..#..##.##.#";
  map += L"#..#..#.###.##.#";
  map += L"#..#..#...#....#";
  map += L"#.....#...#.#..#";
  map += L"#######.###.#..#";
  map += L"#..#..#...#.##.#";
  map += L"#.....#.....#..#";
  map += L"#..#..#...#.#..#";
  map += L"#..##########..#";
  map += L"#..#..#...#....#";
  map += L"#..............#";
  map += L"#.....#...#....#";
  map += L"################";

  // screen initialization.
  initscr();
  cbreak();      // ctrl-c leaves program .
  halfdelay(1);  // non-blocking user input (1 => 1 sec), unline "timeout(3000)" which is half-blocking user input (1 => 1 milisec) .
  noecho();      // no immediate displaying of typed input .
  start_color(); // initialize colors .
  int x,y,xBeg,yBeg,xMax,yMax; // the window dimensions .
  int xMin = 0;
  int yMin = 0;
  getyx(stdscr,y,x);           // current position of cursor in window .
  getbegyx(stdscr,yBeg,xBeg);  // top left corner of window.
  getmaxyx(stdscr,yMax,xMax);  // get width and height ('stdscr' standard window as opposed to created windows) .
  refresh();                   // synchronize memory and screen .
  int screenWidth = xMax;
  int screenHeight = yMax;

  // create window.
  WINDOW *win = newwin(0,0,0,0); // make window .
  box(win, '|', '-'); // make a box .
  mvwprintw(win, yMax/2, xMax/2, "o");
  wrefresh(win);
  move(x, y); // move to position .

  // set up colors .
  init_pair(2, COLOR_WHITE, COLOR_BLUE);  // ceiling color .
  init_pair(3, COLOR_BLACK, COLOR_WHITE); // floor color .
  init_pair(4, COLOR_BLACK, COLOR_GREEN); // display color .
                                          // random wall color .
  srand (time(NULL));
  int myRandom = (int) (rand() % 3);
  switch (myRandom) {
    case 0:
      init_pair(1, COLOR_RED, COLOR_BLACK);
      break;
    case 1:
      init_pair(1, COLOR_YELLOW, COLOR_BLACK);
      break;
    case 2:
      init_pair(1, COLOR_BLUE, COLOR_BLACK);
      break;
  }

  wattron(win, A_BOLD);
  wattron(win, A_REVERSE);

  // set up game loop.
  curs_set(0); // hide the default screen cursor .
  int c = -1;
  int EXIT_KEY = 'q';

  // game loop .
  while ((c=getch()) != EXIT_KEY) { // get user input key .

    float moveFactor = 0;
    switch (c) {
      case 'w': // move forward .
        moveFactor = 1;
        break;
      case 's': // move backward .
        moveFactor = -1;
        break;
      case 'd': // turn right .
        playerDir += 0.1;
        break;
      case 'a': // turn left .
        playerDir -= 0.1;
        break;
      case 'r': // reset .
        playerX = 2.0f;
        playerY = 2.0f;
        playerDir = 0.0f;
        break;
    }
    float dirX = sinf(playerDir);
    float dirY = cosf(playerDir);
    // move .
    playerX += moveFactor * dirX;
    playerY += moveFactor * dirY;

    for (int i = 0; i < screenWidth; i++) { // loop through screen colons.

      // for each column calculate the projected ray angle into world space .
      float rayAngle = (playerDir - fov / 2.0f) + ((float) i / (float) screenWidth) * fov;
      float d2w = 0; // Distance To Wall .
      float stepToWall = 0.1f;
      float i_hat = sinf(rayAngle); // unit vector for ray in player space .
      float j_hat = cosf(rayAngle); 
      bool isHit = false;
      bool isEdge = false;

      // calculate distance to wall (d2w) .
      while (!isHit && d2w < DEPTHMAX) { // step towards wall, then test for bounds .
        d2w += stepToWall;  // increment steps .
        int testX = (int) (playerX + i_hat * d2w);
        int testY = (int) (playerY + j_hat * d2w);
        // test if ray is out of bounds .
        if (testX < 0 || testX >= mapWidth || testY < 0 || testY >= mapHeight) {
          isHit = true;
          d2w = DEPTHMAX;
        } else {
          // ray is inbounds, thus test for cell to be wall block .
          if (map[testY * mapWidth + testX] == '#') {
            isHit = true;

            // get data of edges .
            vector<pair<float, float> > p;
            for (int tx = 0; tx < 2; tx++) {
              for (int ty = 0; ty < 2; ty++) {
                // angle of corner to player's center .
                float vy = (float) testY + ty - playerY;
                float vx = (float) testX + tx - playerX;
                float d = sqrt(vx*vx + vy*vy); 
                float dot = (i_hat * vx / d) + (j_hat * vy / d);
                p.push_back(make_pair(d, dot));
              }
            }
            // sort pairs from closest to farthest .
            sort(p.begin(), p.end(), [] (const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first; });
            // first two or three are closest .
            float fBound = 0.01;
            if (acos(p.at(0).second) < fBound) isEdge = true;
            if (acos(p.at(1).second) < fBound) isEdge = true;
          }
        }
      }
      // print view .
      int ceiling = screenHeight/2.0f - screenHeight/d2w;    // distance to ceiling .
      int floor = screenHeight - ceiling;                    // distance to wall .
      for (int j = 0; j < screenHeight; j++) {
        string symbol = "x";
        if (j < ceiling) { // must be ceiling .
          symbol = ".";
          wattron(win, COLOR_PAIR(2));
        } else if (j > ceiling && j < floor) { // must be wall .
          if (isEdge) {
            symbol = "|"; // edge of block .
          } else {
            symbol = " "; // normal area .
          }
          wattron(win, COLOR_PAIR(1));
        } else { // must be floor .
          symbol = "_";
          wattron(win, COLOR_PAIR(3));
        }
        // print pixels .
        mvwprintw(win, j, i, "%s", symbol.c_str());
      }
      // set character representing player's direction of view .
      char playerDirChar = 'P';
      float offsetQuarter = playerDir > 0 ? M_PI/4 : -(M_PI/4);
      unsigned int dirCode = (int) (2 * (playerDir + offsetQuarter) / M_PI);
      dirCode = dirCode % 4;
      switch(dirCode) {
        case 0:
          playerDirChar = 'A';
          break;
        case 1:
          playerDirChar = '>';
          break;
        case 2:
          playerDirChar = 'V';
          break;
        case 3:
          playerDirChar = '<';
          break;
      }

      // print infos and stats .
      wattron(win, COLOR_PAIR(4));
      mvwprintw(win, yMax-5, xMax-20, "DTW: %f", d2w);       // distance to wall .
      mvwprintw(win, yMax-4, xMax-20, "P_X: %f", playerX);   // player's x coordinate .
      mvwprintw(win, yMax-3, xMax-20, "P_Y: %f", playerY);   // player's y coordinate .
      mvwprintw(win, yMax-2, xMax-20, "P_D: %f", playerDir); // player's direction .

      // print map .
      wattron(win, COLOR_PAIR(4));
      int xOffset = 1;
      int yOffset = 1;
      int mapMax = mapHeight * mapWidth - 1;
      for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
          if ((int) playerX == (mapWidth - j - 1) && (int) playerY == (mapHeight - i - 1)) {
            mvwprintw(win, yMin+yOffset+i, xMin+xOffset+(mapWidth-j), "%c", playerDirChar);
          } else {
            char mapChar = map[mapMax - (i * mapWidth + j)];
            if (mapChar == '.') {
              mapChar = ' '; // character for hallway in map .
              mvwprintw(win, yMin+yOffset+i, xMin+xOffset+(mapWidth-j), "%c", mapChar);
            } else if (mapChar == '#') {
              mapChar = 'O'; // character for wall in map .
              mvwprintw(win, yMin+yOffset+i, xMin+xOffset+(mapWidth-j), "%c", mapChar);
            }
          }
        }
      }
    }

    // refresh window .
    wrefresh(win); 
    usleep(20000);

  }  // end game loop .

  // turn attributes off .
  wattroff(win, COLOR_PAIR(1));
  wattroff(win, A_REVERSE);
  wattroff(win, A_BOLD);

  // wrap up .
  delwin(win); // remove windows .
  clear();
  endwin();  
  return 0;
}
