#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define PCD8544_SCLK 13
#define PCD8544_DIN  11
#define PCD8544_DC   8
#define PCD8544_CS   10
#define PCD8544_RST  9
#define PCD8544_BL   7

#define KEY_LEFT 1
#define KEY_CENTER 2
#define KEY_DOWN 3
#define KEY_UP 4
#define KEY_RIGHT 5

Adafruit_PCD8544 display = Adafruit_PCD8544(PCD8544_SCLK, PCD8544_DIN, PCD8544_DC, PCD8544_CS, PCD8544_RST);
const int tailArraySize = 200;
int tailActiveArraySize = 10;

void setup() {
  pinMode(PCD8544_BL, OUTPUT);

  //48x84 pixels matrix LCD
  display.begin();

  display.setContrast(55);

  display.display();

  for (int j = 0; j < 255; j++) {
    analogWrite(PCD8544_BL, j);
    delay(10);
  }

  display.clearDisplay();

  display.setTextSize(1);

  Serial.begin(9600);
  // if analog input pin 8 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(8));
}

int readKey(int key) {
  int value = analogRead(key);
  if (value < 30) {
    return KEY_LEFT;
  } else if (value < 150) {
    return KEY_CENTER;
  } else if (value < 360) {
    return KEY_DOWN;
  } else if (value < 535) {
    return KEY_UP;
  } else if (value < 760) {
    return KEY_RIGHT;
  } else {
    return 0;
  }
}

void drawPlayer(int* playerPosX, int* playerPosY) {
  display.drawPixel(*playerPosX, *playerPosY, 0xFFFF);
  display.drawPixel(*playerPosX, *playerPosY + 1, 0xFFFF);
  display.drawPixel(*playerPosX + 1, *playerPosY, 0xFFFF);
  display.drawPixel(*playerPosX + 1, *playerPosY + 1, 0xFFFF);
}

void drawTailBlock(int tailPosX, int tailPosY){
  int x = tailPosX;
  int y = tailPosY;
  
  display.drawPixel(x, y, 0xFFFF);
  display.drawPixel(x, y + 1, 0xFFFF);
  display.drawPixel(x + 1, y, 0xFFFF);
  display.drawPixel(x + 1, y + 1, 0xFFFF);
}

void drawTail(int tailArray[tailArraySize][2], int* tailLenght) {
  int i;
  for (i = 0; i < *tailLenght; i++) {
    if (tailArray[i][0] >= 0) {
      drawTailBlock(tailArray[i][0], tailArray[i][1]);
    }
  }
}

void drawPickup(bool* pickupPickedUp, int* pickupPosX, int* pickupPosY){
  if(*pickupPickedUp || *pickupPosX < 0 || *pickupPosY < 0){
    *pickupPosX = 2*random(41);
    *pickupPosY = 2*random(5,23);
    Serial.print(*pickupPosX);
    Serial.print("\t");
    Serial.print(*pickupPosY);
    Serial.print("\n");
  }
  display.drawPixel(*pickupPosX, *pickupPosY, 0xFFFF);
  display.drawPixel(*pickupPosX, *pickupPosY + 1, 0xFFFF);
  display.drawPixel(*pickupPosX + 1, *pickupPosY, 0xFFFF);
  display.drawPixel(*pickupPosX + 1, *pickupPosY + 1, 0xFFFF);
  *pickupPickedUp = false;
}

void drawScoreBoard(){
  int i;
  for (i = 1; i<= 82; i++){
    if(i%2 == 0){
      display.drawPixel(i, 8, 0xFFFF);
    } else{
      display.drawPixel(i, 9, 0xFFFF);
    }
  }
}

int checkPlayer(int* playerPosX, int* playerPosY) {
  if (*playerPosX < 0 ) {
    *playerPosX = 82;
  } else if (*playerPosX > 82 ) {
    *playerPosX = 0;
  } else if (*playerPosY < 10 ) {
    *playerPosY = 46;
  } else if (*playerPosY > 46 ) {
    *playerPosY = 10;
  }
}

void checkInput(int* movDirX, int* movDirY) {
  switch (readKey(A0)) {
    case KEY_LEFT:
      if (*movDirX == 2) {
        break;
      } else {
        *movDirX = -2;
        *movDirY = 0;
        break;
      }
    case KEY_CENTER:
      *movDirX = 0;
      *movDirY = 0;
      break;
    case KEY_DOWN:
      if (*movDirY == -2) {
        break;
      } else {
        *movDirY = 2;
        *movDirX = 0;
        break;
      }
    case KEY_UP:
      if (*movDirY == 2) {
        break;
      } else {
        *movDirY = -2;
        *movDirX = 0;
        break;
      }
    case KEY_RIGHT:
      if (*movDirX == -2) {
        break;
      } else {
        *movDirX = 2;
        *movDirY = 0;
        break;
      }
  }
}

void checkPickupIsPickedUp(int* playerPosX, int* playerPosY, int* pickupPosX, int* pickupPosY, bool* pickupPickedUp){
  if((*playerPosX == *pickupPosX || *playerPosX == *pickupPosX + 1) && (*playerPosY == *pickupPosY || *playerPosY == *pickupPosY + 1)){
    *pickupPickedUp = true;
  }
}

void checkGameOver(int* playerPosX, int* playerPosY, int tailArray[tailArraySize][2], int* score){
  int i;
  for(i=0; i<tailArraySize; i++){
    if((*playerPosX == tailArray[i][0] || *playerPosX == tailArray[i][0] + 1) && (*playerPosY == tailArray[i][1] || *playerPosY == tailArray[i][1] + 1)){
      gameOverScreen(&score);
    }
  }
}

void curtain(){
  delay(1000);
  int i, e;
  for (i=0; i<=46; i++){
    for(e=0; e<=82; e++)
      display.drawPixel(e, i, 0xFFFF);
      display.display();
      delay(15);
   }
}

void gameOverScreen(int** score){
  curtain();
  while(true){
    delay(200);
    display.clearDisplay();
    display.setCursor(14, 10);
    display.println("Game Over!");
    display.setCursor(14, 20);
    display.println("Score:");
    display.setCursor(14, 30);
    display.println(**score);
    display.display();
    delay(200000);
  }
}

void checkWin(int* score){
  if(*score >= tailArraySize * 10){
    winScreen(&score);
  }
}

void winScreen(int** score){
  curtain();
  while(true){
    delay(200);
    display.clearDisplay();
    display.setCursor(14, 10);
    display.println("You Win!");
    display.setCursor(14, 20);
    display.println("Score:");
    display.setCursor(14, 30);
    display.println(**score);
    display.display();
    delay(200000);
  }
}

int movDirX = 2;
int movDirY = 0;
int playerPosX = 0;
int playerPosY = 2*random(6,21);;
int pickupPosX = -1;
int pickupPosY = -1;
int movSpeed = 1000;
int e = 1;
bool pickupPickedUp = false;
int tailLenght = (tailArraySize - tailArraySize) + tailActiveArraySize;
int score = 0;

//Initial tail
int tailArray[tailArraySize][2] = {{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2},{-2,-2}};

void loop() {
  display.clearDisplay();
  
  checkInput(&movDirX, &movDirY);

  tailArray[tailLenght-e][0] = playerPosX;
  tailArray[tailLenght-e][1] = playerPosY;

  e++;
  if(e == tailLenght+1){
    e = 1;
  }

  playerPosX += movDirX;
  playerPosY += movDirY;

  checkPlayer(&playerPosX, &playerPosY);

  drawPlayer(&playerPosX, &playerPosY);
  drawTail(tailArray, &tailLenght);
  drawPickup(&pickupPickedUp, &pickupPosX, &pickupPosY);
  drawScoreBoard();
  
  checkPickupIsPickedUp(&playerPosX, &playerPosY, &pickupPosX, &pickupPosY, &pickupPickedUp);
  
  if(pickupPickedUp){
    tailLenght++;
    score += 10;
    tailArray[tailLenght-1][0] = playerPosX;
    tailArray[tailLenght-1][1] = playerPosY;
    e++;
  }

  display.setCursor(0, 0);
  display.println(score);
  if(!pickupPickedUp){
    checkGameOver(&playerPosX, &playerPosY, tailArray, &score);
  }
  
  checkWin(&score);
  
  display.display();
  
  delay(movSpeed / 8);
}

