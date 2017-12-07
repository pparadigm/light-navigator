#include "LedControlMS.h"

LedControl lc=LedControl(12,11,10,1);

// levels
const int floor1[8][8] = {{0,1,0,0,0,1,0,0},
                          {0,1,0,1,0,0,0,1},
                          {0,1,0,1,1,1,1,1},
                          {0,0,0,1,0,0,1,0},
                          {1,1,0,1,0,1,1,0},
                          {0,0,0,1,0,1,0,0},
                          {0,1,0,1,0,1,0,1},
                          {0,1,0,0,0,0,0,1}};

const int NUM_LEVELS = 1;
typedef const int (*mapsPtr)[8];
mapsPtr maps[NUM_LEVELS] = {floor1};

int curLevel = 0;
bool changeLevel;

void setup() {
  // wake up Arduino
  lc.shutdown(0,false);
  // set brightness
  lc.setIntensity(0,4);
  // clearDisplay
  lc.clearDisplay(0);

  changeLevel = true;
}

void loop() {
  if(changeLevel) {
    curLevel += 1;
    displayMap();
  }
}

// purpose: display an 8x8 map
// pre:     none
// post:    map is displayed on 8x8 LED array
// param:   curLevel - the index of the map to display
// return:  none
void displayMap() {
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      bool state = maps[curLevel-1][i][j]; // probably curLevel should be passed in?
      lc.setLed(0, i, j, state);
    }
  }
}

