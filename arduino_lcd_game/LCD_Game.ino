#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 1, en = 2, d4 = 3, d5 = 4, d6 = 5, d7 = 6, sw = 7, xPin = A0, yPin = A1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long resetTimer = -2000;

// player
int xChg, yChg;
int pX, pY, pXlast, pYlast;
const int pBound = 8;

// bullet
int bX = -1, bY = -1;
long bulletTimer;
int bulletRate = 1000;

// objects
const int numObjects = 5;
int oX[numObjects];
int oY[numObjects];
const int oSpawn = 3000;
long oTimer;
int spawnY;

// explosions
int eX[numObjects];
int eY[numObjects];
long expTimer[numObjects];
const int expRate = 500;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  initObjects();

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(sw, INPUT_PULLUP); 
}

void loop() {
  const long curTime = millis();
  drawExp("X", curTime);
  delay(150);
  if (curTime - resetTimer < 2000) {
     return;
  }

  updateObjects(curTime);
  updatePlayer();
  updateBullet(curTime);

  
  drawBullet("-");
  drawObjects("O");
  drawPlayer(">");

  collisions(curTime);
}

// initialize objects
void initObjects() {
  for (int i = 0; i < numObjects; i++) {
    oX[i] = -1;
    oY[i] = -1;
    eX[i] = -1;
    eY[i] = -1;
  }
}



// shift objects by one square
void updateObjects(const long curTime) {
  spawnObject(curTime);
  for (int i = 0; i < numObjects; i++) {
    if (validIndex(oX[i], oY[i]) || oX[i] == 16) {
      oX[i]--;
    }
  }
}

// spawns a new object based on object spawn timer
void spawnObject(const long curTime) {
  if (curTime - oTimer >= oSpawn) {

    // spawn object at next available spot
    for (int i = 0; i < numObjects; i++) {
      if (!validIndex(oX[i], oY[i])) {
         spawnY = round(random(2));
         oX[i] = 16;
         oY[i] = spawnY;
         break;
      }
    }

    oTimer = curTime;
  }
}

void updatePlayer() {
  xChg = analogRead(xPin);
  yChg = analogRead(yPin);

  // update position of player
  if (xChg < 300 && pX < pBound) {
    pX++;
  } else if (xChg > 700 && pX > 0) {
    pX--;
  } else if (yChg < 350 && pY > 0) {
    pY--;
  } else if (yChg > 500 && pY < 1) {
    pY++;
  }
}

void updateBullet(long curTime) {
  if (digitalRead(sw) == 0 && curTime - bulletTimer >= bulletRate && !validIndex(bX, bY)) {
    bX = pX;
    bY = pY;
    bulletTimer = curTime;
  }

  bX++;
}

// COLLISION ----------------------------------------------------------------------------

void collisions(const long curTime) {
  for (int i = 0; i < numObjects; i++) {
    // collided with player
    if (pX == oX[i] && pY == oY[i] || pXlast == oX[i] && pYlast == oY[i]) {
      addExplosion(oX[i], oY[i], curTime);
      oX[i] = -1;
      oY[i] = -1;
      draw(bX, bY, " ", -1, -1);
      bX = -1;
      bY = -1;
      resetTimer = curTime;
    }

    // collided with bullet
    if (bX == oX[i] && bY == oY[i] || bX - 1 == oX[i] && bY == oY[i]) {
      addExplosion(oX[i], oY[i], curTime);
      bX = -1;
      bY = -1;
      oX[i] = -1;
      oY[i] = -1;
    }
  }
}

void addExplosion(const int x, const int y, const long curTime) {
  for (int i = 0; i < numObjects; i++) {
    if (!validIndex(eX[i], eY[i])) {
      eX[i] = x;
      eY[i] = y;
      expTimer[i] = curTime;
      break;
    }
  }
}

// DRAWING ------------------------------------------------------------------------------

void drawBullet(String symbol) {
  draw(bX, bY, symbol, bX - 1, bY);
}

void drawObjects(String symbol) {
  for (int i = 0; i < numObjects; i++) {
    draw(oX[i], oY[i], symbol, oX[i] + 1, oY[i]);
  }
}

void drawPlayer(String symbol) {
  draw(pX, pY, symbol, pXlast, pYlast);
  pXlast = pX;
  pYlast = pY;
}

void drawExp(String symbol, const long curTime) {
  for (int i = 0; i < numObjects; i++) {
    draw(eX[i], eY[i], symbol, -1 , -1);

    if (curTime - expTimer[i] >= expRate) {
      draw(eX[i], eY[i], " ", -1 , -1);
      eX[i] = -1;
      eY[i] = -1;
    }
  }
}

void draw(const int x, const int y, const String symbol, const int prevX, const int prevY) {
  if (validIndex(x, y)) {
    lcd.setCursor(x, y);
    lcd.print(symbol);
  }

  if (validIndex(prevX, prevY) && (x != prevX || y != prevY)) {
    lcd.setCursor(prevX, prevY);
    lcd.print(" ");
  }
}

// INDEX HELPERS ------------------------------------------------------------------------------

bool validIndex(const int x, const int y) {
  return x >= 0 && x <= 15 && y >= 0 && y <= 1;
}
