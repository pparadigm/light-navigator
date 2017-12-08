#include "LedControlMS.h"

LedControl lc=LedControl(12,11,10,1);
const double TICK = 1000; // TICK is a number that the Arduino takes ~1 second to count up to

// levels

const int floor0[8][8] = {{1,1,1,1,1,1,1,1},
                          {1,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,1},
                          {1,0,0,0,0,0,0,1},
                          {1,1,1,1,1,1,1,1}};

const int floor1[8][8] = {{0,1,0,0,0,1,0,0},
                          {0,1,0,1,0,0,0,1},
                          {0,1,0,1,1,1,1,1},
                          {0,0,0,1,0,0,1,0},
                          {1,1,0,1,0,1,1,0},
                          {0,0,0,1,0,1,0,0},
                          {0,1,0,1,0,1,0,1},
                          {0,1,0,0,0,0,0,1}};

const int NUM_LEVELS = 2;
typedef const int (*mapsPtr)[8];
mapsPtr maps[NUM_LEVELS] = {floor0, floor1};

int startLevel = 1, curLevel, start[2] = {}, goal[2] = {}, pX, pY, count;
bool changeLevel, pipState, goalState;
char go;

void setup() {
  // wake up Arduino
  lc.shutdown(0,false);
  // set brightness
  lc.setIntensity(0,4);
  // clearDisplay
  lc.clearDisplay(0);

  changeLevel = true;
  curLevel = startLevel;
}

void loop() {
  while(true) {
    // load map
    if(changeLevel) {
      displayMap(curLevel);
  
      // set initial level values
      // eventually these positions could be random from a list, and would depend on the map
      start[0] = 0, start[1] = 0; // y, x
      goal[0] = 3, goal[1] = 7;   // y, x
      pX = start[1];
      pY = start[0];
      changeLevel = false;
      count = 0;
      pipState = false, goalState = false;
    }
    delay(TICK/4);
  
    // blink player character
    if (count % 2) {
      pipState = !pipState;
      lc.setLed(0, pX, pY, pipState);
    }

    // blink goal
    goalState = !goalState;
    lc.setLed(0, goal[0], goal[1], goalState);

    // see if player moved and react
    if (count % 4) {
      go = getDirection();
      if (go != 'C') {
        placePip(pX, pY, go);
        // if pip is at the goal
        if (pX == goal[1] && pY == goal[0]) {
          winAnim(pX, pY);
        }
      }
    }
    
    count = (count + 1) % 60; // 60 is an arbitrary # with lots of factors
  }
}

// purpose: display an 8x8 map
// pre:     none
// post:    map is displayed on 8x8 LED array
// param:   level - the index of the map to display
// return:  none
void displayMap(int level) {
  for(int y = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++) {
      bool state = maps[level][y][x];
      lc.setLed(0, y, x, state);
    }
  }
}

// purpose: read input from a file
// pre:     none
// post:    none
// param:   none
// return:  N, W, S, E, or C
char getDirection() {
  // STUB
  // talk to whatever: txt file or joystick
  return 'C';
}

// purpose: "move" the pip on the board
// pre:     non
// post:    pip coordinates (x and y) are updated, invalid dir is ignored
// param:   x - x-coordinate of pip
//          y - y-coordinate of pip
//          dir - a char that directs the movement of a pip
// return:  none
void placePip(int &x, int &y, char dir) {
  // STUB
  // using dir, change x or y
}

// purpose: display an animation centered on the pip
// pre:     none
// post:    none
// param:   x - x-coordinate of animation epicenter
//          y - y-coordinate of animation epicenter
// return:  none
void winAnim(int x, int y) {
  // STUB
  // show win animation
}

