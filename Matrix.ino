#include "LedControl.h"

#define PLAYER 3
#define ENEMY 2

LedControl lc=LedControl(12,11,10,1);

boolean inGame = true;

unsigned long scor = 0;
byte enemyRows = 2;

byte pixels[8][8];
byte joc[13][8];

int x = 2;
int y = 3;

int speedGame = 500;

unsigned long lastRead = 0;
unsigned long lastSp = 0;
unsigned long lastUp = 0;
unsigned long timeEnd = 0;

int currentPos = 3;

int jsPin = 1;

void resetGame(){
  
  speedGame = 500;
  scor = millis();
  enemyRows = 2;
  
  // initialize the pixel matrix:
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      pixels[x][y] = HIGH;
    } 
  }
  
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 8; j++) {
      joc[i][j] = 0;
    } 
  }

  //jucator
  currentPos = 3;
  joc[0][currentPos-1] = joc[0][currentPos] = joc[0][currentPos+1] = PLAYER;
  joc[1][currentPos] = PLAYER;
  joc[2][currentPos-1] = joc[2][currentPos] = joc[2][currentPos+1] = PLAYER;
  joc[3][currentPos] = PLAYER;
  
}

void setup() {
  
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,0);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  resetGame();
  
}

void loop() {
  
  if(inGame){
    upGame();
    readSensors();  
  }
  else{
    endScreen();
  }
  
  refreshScreen();
  
}


void checkInput(){
  
  int act = analogRead(jsPin);
    
  if(act < 500 || act > 550){
    inGame = true;
    resetGame();
  }
    
}


void afiseazaScor(int scor){

  if(scor > 99)
    scor = 99;
  
  int unit = scor % 10;
  int zecim = scor / 10;

  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      pixels[i][j] = HIGH;
  
  switch(zecim){
    case 0: pixels[1][3] = pixels[2][3] = pixels[3][3] = pixels[4][3] = pixels[5][3] = pixels[1][1] = pixels[2][1] = pixels[3][1] = pixels[4][1] = pixels[5][1] = pixels[1][2] = pixels[5][2] = 0; break;
    case 1: pixels[1][3] = pixels[2][3] = pixels[3][3] = pixels[4][3] = pixels[5][3] = 0; break;
    case 2: pixels[1][3] = pixels[2][3] = pixels[3][3] = pixels[5][3] = pixels[1][1] = pixels[3][1] = pixels[4][1] = pixels[5][1] = pixels[1][2] = pixels[5][2] = pixels[3][2] = 0; break;
    case 3: pixels[1][3] = pixels[2][3] = pixels[3][3] = pixels[4][3] = pixels[5][3] = pixels[1][1] = pixels[3][1] = pixels[5][1] = pixels[1][2] = pixels[5][2] = pixels[3][2] = 0; break;
    case 4: pixels[1][3] = pixels[2][3] = pixels[3][3] = pixels[4][3] = pixels[5][3] = pixels[1][1] = pixels[2][1] = pixels[3][1] = pixels[3][2] = 0; break;
    case 5: pixels[1][3] = pixels[3][3] = pixels[4][3] = pixels[5][3] = pixels[1][1] = pixels[2][1] = pixels[3][1] = pixels[5][1] = pixels[1][2] = pixels[5][2] = pixels[3][2] = 0; break;
    case 6: pixels[3][3] = pixels[4][3] = pixels[5][3] = pixels[1][1] = pixels[2][1] = pixels[3][1] = pixels[4][1] = pixels[5][1] = pixels[5][2] = pixels[3][2] = pixels[1][2] = pixels[1][3] = 0; break;
    case 7: pixels[1][3] = pixels[2][3] = pixels[3][3] = pixels[4][3] = pixels[5][3] = pixels[1][1] = pixels[1][2] = 0; break;
    case 8: pixels[1][3] = pixels[2][3] = pixels[3][3] = pixels[4][3] = pixels[5][3] = pixels[1][1] = pixels[2][1] = pixels[3][1] = pixels[4][1] = pixels[5][1] = pixels[1][2] = pixels[5][2] = pixels[3][2] = 0; break;
    case 9: pixels[1][3] = pixels[2][3] = pixels[3][3] = pixels[4][3] = pixels[5][3] = pixels[1][1] = pixels[2][1] = pixels[3][1] = pixels[1][2] = pixels[3][2] = pixels[5][1] = pixels[5][2] = 0; break;
  }
  
  switch(unit){
    case 0: pixels[1][7] = pixels[2][7] = pixels[3][7] = pixels[4][7] = pixels[5][7] = pixels[1][5] = pixels[2][5] = pixels[3][5] = pixels[4][5] = pixels[5][5] = pixels[1][6] = pixels[5][6] = 0; break;
    case 1: pixels[1][7] = pixels[2][7] = pixels[3][7] = pixels[4][7] = pixels[5][7] = 0; break;
    case 2: pixels[1][7] = pixels[2][7] = pixels[3][7] = pixels[5][7] = pixels[1][5] = pixels[3][5] = pixels[4][5] = pixels[5][5] = pixels[1][6] = pixels[5][6] = pixels[3][6] = 0; break;
    case 3: pixels[1][7] = pixels[2][7] = pixels[3][7] = pixels[4][7] = pixels[5][7] = pixels[1][5] = pixels[3][5] = pixels[5][5] = pixels[1][6] = pixels[5][6] = pixels[3][6] = 0; break;
    case 4: pixels[1][7] = pixels[2][7] = pixels[3][7] = pixels[4][7] = pixels[5][7] = pixels[1][5] = pixels[2][5] = pixels[3][5] = pixels[3][6] = 0; break;
    case 5: pixels[1][7] = pixels[3][7] = pixels[4][7] = pixels[5][7] = pixels[1][5] = pixels[2][5] = pixels[3][5] = pixels[5][5] = pixels[1][6] = pixels[5][6] = pixels[3][6] = 0; break;
    case 6: pixels[3][7] = pixels[4][7] = pixels[5][7] = pixels[1][5] = pixels[2][5] = pixels[3][5] = pixels[4][5] = pixels[5][5] = pixels[5][6] = pixels[3][6] = pixels[1][6] = pixels[1][7] = 0; break;
    case 7: pixels[1][7] = pixels[2][7] = pixels[3][7] = pixels[4][7] = pixels[5][7] = pixels[1][5] = pixels[1][6] = 0; break;
    case 8: pixels[1][7] = pixels[2][7] = pixels[3][7] = pixels[4][7] = pixels[5][7] = pixels[1][5] = pixels[2][5] = pixels[3][5] = pixels[4][5] = pixels[5][5] = pixels[1][6] = pixels[5][6] = pixels[3][6] = 0; break;
    case 9: pixels[1][7] = pixels[2][7] = pixels[3][7] = pixels[4][7] = pixels[5][7] = pixels[1][5] = pixels[2][5] = pixels[3][5] = pixels[1][6] = pixels[3][6] = pixels[5][5] = pixels[5][6] = 0; break;
  }
  
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 8; j++){
      int aux = pixels[i][j];
      pixels[i][j] = pixels[7-i][7-j];
      pixels[7-i][7-j] = aux;
    }
  
}


void endScreen(){
  
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      pixels[i][j] = LOW;
  
  if(millis() - timeEnd > 1500){
    afiseazaScor(scor);
    checkInput();

  }
  else if(millis() - timeEnd > 1250){
    for(int i = 0; i < 8; i++)
      for(int j = 0; j < 8; j++)
        pixels[i][j] = HIGH;
    
  }
  else if(millis() - timeEnd > 1000){
    for(int i = 0; i < 8; i++)
      for(int j = 0; j < 8; j++)
        pixels[i][j] = LOW;
    
  }
  else if(millis() - timeEnd > 750){
    for(int i = 0; i < 8; i++)
      for(int j = 0; j < 8; j++)
        pixels[i][j] = HIGH;
  }
  else if(millis() - timeEnd > 500){
    for(int i = 0; i < 8; i++)
      for(int j = 0; j < 8; j++)
        pixels[i][j] = LOW;
  }
  else if(millis() - timeEnd > 250){
    for(int i = 0; i < 8; i++)
      for(int j = 0; j < 8; j++)
        pixels[i][j] = HIGH;
  }
  
}

void checkCollision(){
 
  if(joc[3][currentPos] == ENEMY || joc[2][currentPos-1] == ENEMY 
     || joc[2][currentPos+1] == ENEMY || joc[0][currentPos-1] == ENEMY
     || joc[0][currentPos+1] == ENEMY){
        
    scor = millis() - scor; // initial, scor = millis() de la inceputul rundei
    scor /= 5000;           // scor = 1 punct pentru fiecare 5 secunde jucate
    
    timeEnd = millis();
    inGame = false;
    
  } 
  
}


void spawnEnemies(){
  
  int randNum = random(enemyRows);
  
  if(randNum == 0){
    joc[8][0] = joc[8][1] = joc[8][2] = ENEMY;
    joc[9][1] = ENEMY;
    joc[10][0] = joc[10][1] = joc[10][2] = ENEMY;
    joc[11][1] = ENEMY;
  }
  else if(randNum == 1){
    joc[8][5] = joc[8][6] = joc[8][7] = ENEMY;
    joc[9][6] = ENEMY;
    joc[10][5] = joc[10][6] = joc[10][7] = ENEMY;
    joc[11][6] = ENEMY;
    
  }
  else if(randNum == 2){
    joc[8][3] = joc[8][4] = joc[8][5] = ENEMY;
    joc[9][4] = ENEMY;
    joc[10][3] = joc[10][4] = joc[10][5] = ENEMY;
    joc[11][4] = ENEMY;
    
  }
  else if(randNum == 3){
    joc[8][2] = joc[8][3] = joc[8][1] = ENEMY;
    joc[9][2] = ENEMY;
    joc[10][2] = joc[10][3] = joc[10][1] = ENEMY;
    joc[11][2] = ENEMY;
    
  }
  
  if((speedGame > 101 && enemyRows <= 3) || 
     (speedGame > 161 && enemyRows == 4))
    speedGame -= speedGame/8;
  else if((millis() - scor > 30000 && enemyRows == 2) ||
          (millis() - scor > 100000 && enemyRows == 3)){
    enemyRows++;
    Serial.println(enemyRows);
    speedGame = 270;
  }
  
}


void avanseaza(){
  
  for(int i = 0; i < 12; i++)
    for(int j = 0; j < 8; j++){
      
      if((joc[i][j] != PLAYER
         && !(i == 1 && j == currentPos - 1) && !(i == 1 && j == currentPos + 1))
         || (joc[i][j] == PLAYER && joc[i+1][j] == ENEMY))
        joc[i][j] = joc[i+1][j];
      
    }
    
  // pune informatia pe matricea de afisare  
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++){
      if(joc[i][j] == 0)
        pixels[i][j] = 1;
      else
        pixels[i][j] = 0;
    }
  
}


void upGame(){
  
  if(millis() - lastSp > speedGame*10){
    spawnEnemies();
    
    lastSp = millis();
  }
  
  
  if(millis() - lastUp > speedGame){  
    avanseaza();    
    checkCollision();
    
    lastUp = millis();
  }
  
  
}

void mutaPlayer(int poz){
  
  
  joc[0][currentPos-1] = joc[0][currentPos] = joc[0][currentPos+1] = 0;
  joc[1][currentPos] = 0;
  joc[2][currentPos-1] = joc[2][currentPos] = joc[2][currentPos+1] = 0;
  joc[3][currentPos] = 0;
  
  // muta player
  currentPos += poz;
  
  checkCollision();
  
  joc[0][currentPos-1] = joc[0][currentPos] = joc[0][currentPos+1] = PLAYER;
  joc[1][currentPos] = PLAYER;
  joc[2][currentPos-1] = joc[2][currentPos] = joc[2][currentPos+1] = PLAYER;
  joc[3][currentPos] = PLAYER;
  
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
    {
      if(joc[i][j] == 0)
        pixels[i][j] = 1;
      else
        pixels[i][j] = 0;
    }
}

void readSensors() {
  
  if(millis() - lastRead > 100){
    
    int act = analogRead(jsPin);
    
    if(act < 510 && currentPos > 1)
      mutaPlayer(-1);
    else if(act > 522 && currentPos < 6)
      mutaPlayer(1);
    
    lastRead = millis();
  }
  

}

void refreshScreen() {
  
  for (int row=0; row<8; row++){
    for (int col=0; col<8; col++){
      if(pixels[row][col] == LOW)
        lc.setLed(0,col,row,true); // turns on LED at col, row
      else
        lc.setLed(0,col,row,false); // turns off LED at col, row
    }
  }

}
