#include "LedControlMS.h"

 
LedControl lc=LedControl(12,11,10,1);

unsigned long delaytime=10;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,4);
  /* and clear the display */
  lc.clearDisplay(0);
}

void tick(){
  delay(delaytime);
}

bool blinkGoal(bool b){
    if(b){
      lc.setLed(0,7, 3, true); }//end on
    else{
      lc.setLed(0,7, 3, false); }//end off 
      
     b = !b;   
     return b;   
}

void updatePC(int playerX, int playerY){

  //not yet implemented
}

//Displays maze, then loops to blink player and end point
void writeArduinoMaze()
{
   byte level1[8]={B00001000, B11101011, B00000000, B01111110, B00100000 , B10101110,
   B00111000, B01100011};
  lc.setRow(0,0,level1[0]);
  lc.setRow(0,1,level1[1]);
  lc.setRow(0,2,level1[2]);
  lc.setRow(0,3,level1[3]);
  lc.setRow(0,4,level1[4]);
  lc.setRow(0,5,level1[5]);
  lc.setRow(0,6,level1[6]);
  lc.setRow(0,7,level1[7]);
 
  bool PCState, goalState = true;
  int playerX, playerY = 0; //not yet implemented
  int count = 0;
  
  while(true)
  {
    tick();
    count = (count+1) % 60;

    if(count % 30 == 0){
      //not implemented yet, blink player;
      }
   
    if(count % 15 == 0 ){
      goalState = blinkGoal(goalState); }

    if(count % 60 == 0) {
      updatePC(playerX, playerY);
    }
  
   }
}

void loop() { 
  
  writeArduinoMaze();
 
}