#include "LedControlMS.h"

LedControl lc=LedControl(12,11,10,1);

// levels
const int level1[8][8] = {{0,1,0,0,0,1,0,0},
                          {0,1,0,1,0,0,0,1},
                          {0,1,0,1,1,1,1,1},
                          {0,0,0,1,0,0,1,0},
                          {1,1,0,1,0,1,1,0},
                          {0,0,0,1,0,1,0,1},
                          {0,1,0,1,0,1,0,1},
                          {0,1,0,0,0,0,0,1}};

const int levels[] = {level1};

void setup() {
  // wake up Arduino
  lc.shutdown(0,false);
  // set brightness
  lc.setIntensity(0,4);
  // clearDisplay
  lc.clearDisplay(0);

  int choice = pickMap();
  int levelMap = levels[choice];

  // draw map
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      setLED(0, i, j, levelMap[i][j]);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

// sets the map
int pickMap() {
  // STUB
  return 0;
}

