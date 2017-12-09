#include "LedControlMS.h"
#include "arduino.h"

LedControl lc=LedControl(12,11,10,1);
const double TICK = 1000; // TICK is a number that the Arduino takes ~1 second to count up to

// levels
const int floorOff[8][8] = {
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}};
                          
const int floorOn[8][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1}};

const int floorRoom[8][8] = {
  {1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1}};

const int floor1[8][8] = {
  {0,1,0,0,0,1,0,0},
  {0,1,0,1,0,0,0,1},
  {0,1,0,1,1,1,1,1},
  {0,0,0,1,0,0,1,0},
  {1,1,0,1,0,1,1,0},
  {0,0,0,1,0,1,0,0},
  {0,1,0,1,0,1,0,1},
  {0,1,0,0,0,0,0,1}};


const int NUM_LEVELS = 4;
typedef const int (*mapsPtr)[8];
mapsPtr maps[NUM_LEVELS] = {floorOn, floorOff, floorRoom, floor1};

int startLevel = 3, curLevel, start[2] = {}, goal[2] = {}, pX, pY, count;
bool changeLevel, pipState, goalState, collisionOn = true;
char go;

void setup() {
  // wake up Arduino
  lc.shutdown(0,false);
  // set brightness
  lc.setIntensity(0,4);
  // clearDisplay
  lc.clearDisplay(0);

  // set up switch pins
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  
  // set up game
  changeLevel = true;
  curLevel = startLevel;
}

void loop() {
  while(true) {
    // load map
    if(changeLevel) {
      if (curLevel >= NUM_LEVELS) {
        curLevel = startLevel;
      }
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
    delay(TICK/6);
  
    // blink player character
    if (count % 2 == 0) {
      pipState = !pipState;
      lc.setLed(0, pY, pX, pipState);
    }

    // blink goal
    goalState = !goalState;
    lc.setLed(0, goal[0], goal[1], goalState);

    // see if pip moved and react
    if (count % 5 == 0) {
      go = getDirection();
      if (go != 'C') {
        placePip(pY, pX, go);
        
        // if pip is at the goal, win
        if (pX == goal[1] && pY == goal[0]) {
          winAnim(pY, pX);
          curLevel++;
          changeLevel = true;
        }

        // if pip is at a wall, teleport back to start
        if (collisionOn) {
          if (maps[curLevel][pY][pX] == 1) {
            pY = start[0];
            pX = start[1];
          }
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

// purpose: "move" the pip on the board
// pre:     none
// post:    pip coordinates (x and y) are updated, invalid dir is ignored
// param:   x - x-coordinate of pip
//          y - y-coordinate of pip
//          dir - a char that directs the movement of a pip
// return:  none
void placePip(int &y, int &x, char dir) {
  // turn off current location
  lc.setLed(0, y, x, maps[curLevel][y][x]);
  
  // pick new location
  switch(dir) {
    // go north
    case 'N':
      y--;
      if (y < 0) {
        y = 0;
      }
      break;
    // go west
    case 'W':
      x--;
      if (x < 0) {
        x = 0;
      }
      break;
    // go south
    case 'S':
      y++;
      if (y > 7) {
        y = 7;
      }
      break;
    // go east
    case 'E':
      x++;
      if (x > 7) {
        x = 7;
      }
      break;
    // stay
    default:
      // do nothin'
      break;
  }
}

// purpose: display an animation centered on the pip
// pre:     none
// post:    none
// param:   x - x-coordinate of animation epicenter
//          y - y-coordinate of animation epicenter
// return:  none
void winAnim(int y, int x) {
  // STUB
  // show win animation
}

// purpose: get direction from switch
// pre:     none
// post:    none
// param:   none
// return:  N, W, S, E, or C
char getDirection() {
  int value6, value7, value8, value9 = 0;
  char pipDirection;

  // check pins for direction
  value6 = digitalRead(6);
  value7 = digitalRead(7);
  value8 = digitalRead(8);
  value9 = digitalRead(9); 
  
  // make direction usable
  if(value6 == LOW) {
    pipDirection = 'S';
  } else if(value7 == LOW) {
    pipDirection = 'E';
  } else if(value8 == LOW) {
    pipDirection = 'W';
  } else if(value9 == LOW) {
    pipDirection = 'N';
  } else {
    pipDirection = 'C';
  }
  
  return pipDirection;
}

