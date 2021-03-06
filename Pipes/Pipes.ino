#include <M5Stack.h>
#include "utility/MPU9250.h"
#include "Pipe.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BORDER_LENGTH 220
#define X_OFFSET 10
#define Y_OFFSET 10
#define RADIUS 5
#define PIPE_WIDTH 10

#define WIDTH_UNIT ((float)(BORDER_LENGTH)/GRID_WIDTH)
#define HEIGHT_UNIT ((float)(BORDER_LENGTH)/GRID_HEIGHT)


MPU9250 IMU;
int xRot, yRot, zRot;
long oldTime, curTime, startTime;

Coordinate2D start(0,0);

Coordinate2D goal(3,3);

PipesGameField gameField(start, goal);

long drawRate = 1000;
long lastDrawTime;

void setup()
{  
  M5.begin();
  Wire.begin();
  Serial.begin(115200);
  IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);
  IMU.initMPU9250();

  Coordinate2D coord(1,1);

  startTime = millis();
  gameField.setPipe(Pipe::PipeT(), coord);
}

void loop() 
{
  M5.update();

  // handle input

  //button A pressed
  if (M5.BtnA.wasPressed()){
    for(int i = 0; i < GRID_WIDTH; i++){
      for(int j = 0; j < GRID_HEIGHT; j++){
        gameField.grid[i][j].RotateLeft();
      }
    }
  }

  if (M5.BtnB.wasPressed()){
    for(int i = 0; i < GRID_WIDTH; i++){
      for(int j = 0; j < GRID_HEIGHT; j++){
        gameField.grid[i][j].RotateRight();
      }
    }
  }
  

  
  if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) 
  {  
    IMU.readGyroData(IMU.gyroCount);  // Read the x/y/z adc values
    IMU.getGres();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    IMU.gx = (float)IMU.gyroCount[0] * IMU.gRes;
    IMU.gy = (float)IMU.gyroCount[1] * IMU.gRes;
    IMU.gz = (float)IMU.gyroCount[2] * IMU.gRes;

    oldTime = curTime;
    curTime = millis();

    long timeDif = curTime - oldTime;

    xRot += (int)(IMU.gx * timeDif / 1000.);
    yRot += (int)(IMU.gy * timeDif / 1000.);
    zRot += (int)(IMU.gz * timeDif / 1000.);

    if (xRot < 0) 
    {
      xRot = 360 + xRot;
    }
    else if(xRot >= 360) 
    {
      xRot %= 360;
    }

    if (yRot < 0) 
    {
      yRot = 360 + yRot;
    }
    else if(yRot >= 360) 
    {
      yRot %= 360;
    }

    if (zRot < 0) 
    {
      zRot = 360 + zRot;
    }
    else if(zRot >= 360) 
    {
      zRot %= 360;
    }
    
    int x=64+10;
    int y=128+20;
    int z=192+30;
    
    
  }

  long t = millis();
  if(t - lastDrawTime >  drawRate){
    draw();
    lastDrawTime = t;
  }
  drawTime();
}

void drawTime() {

  M5.Lcd.setCursor(240, 10); M5.Lcd.print("TIME");
  M5.Lcd.setCursor(240, 30); M5.Lcd.print(millis() - startTime);
}

void draw()
{
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
  
  for(int x=0; x <= GRID_WIDTH; x++) 
  {
    int tempX = (int) (x*(float)(BORDER_LENGTH)/GRID_WIDTH) + X_OFFSET;
    M5.Lcd.drawLine(tempX, Y_OFFSET, tempX, SCREEN_HEIGHT - (SCREEN_HEIGHT - BORDER_LENGTH) + Y_OFFSET, RED);
  }

  for(int y=0; y <= GRID_HEIGHT; y++) 
  {
    int tempY = (int) (y*(float)(BORDER_LENGTH)/GRID_HEIGHT) + Y_OFFSET;
    M5.Lcd.drawLine(X_OFFSET, tempY, SCREEN_WIDTH - (SCREEN_WIDTH - BORDER_LENGTH) + X_OFFSET, tempY, RED);
  }

  drawStart();
  drawGoal();

  for(int x=0; x < GRID_WIDTH; x++) {
    for(int y=0; y < GRID_HEIGHT; y++) {
      drawPipe(x, y);
    }
  }
}

void drawStart() {

  Coordinate2D coord = gameField.start;
  int tempX = (int) (coord.x*(float)(BORDER_LENGTH)/GRID_WIDTH) + X_OFFSET;
  int tempY = (int) (coord.y*(float)(BORDER_LENGTH)/GRID_HEIGHT) + Y_OFFSET;

  M5.Lcd.fillRect(tempX, tempY, WIDTH_UNIT, HEIGHT_UNIT, YELLOW);
}

void drawGoal() {

  Coordinate2D coord = gameField.goal;
  int tempX = (int) (coord.x*(float)(BORDER_LENGTH)/GRID_WIDTH) + X_OFFSET;
  int tempY = (int) (coord.y*(float)(BORDER_LENGTH)/GRID_HEIGHT) + Y_OFFSET;

  M5.Lcd.fillRect(tempX, tempY, WIDTH_UNIT, HEIGHT_UNIT, CYAN);
}

void drawPipe(int x, int y) {

  Pipe p = gameField.getPipe(x, y);

  drawConnection(x,y,p);

  int xCoord = (int) (x*WIDTH_UNIT + WIDTH_UNIT/2 - RADIUS) + X_OFFSET;
  int yCoord = (int) (y*HEIGHT_UNIT + HEIGHT_UNIT/2 - RADIUS) + Y_OFFSET;

  if(p.visited) {
    M5.Lcd.fillRoundRect(xCoord, yCoord, 2*RADIUS, 2*RADIUS, RADIUS, BLUE);
  }
  else {
    M5.Lcd.fillRoundRect(xCoord, yCoord, 2*RADIUS, 2*RADIUS, RADIUS, RED);
  }
}

void drawConnection(int x, int y, Pipe p) {

  for(int dir=0; dir < NUM_CON; dir++) {
    if(p.connections[dir]) {
      switch(dir) {
        case eConnectionType::North:
          drawNorth(x,y);
          break;
        case eConnectionType::East:
          drawEast(x,y);
          break;
        case eConnectionType::West:
          drawWest(x,y);
          break;
        case eConnectionType::South:
          drawSouth(x,y);
          break;
      }
    }
  }
}

void drawNorth(int x, int y) {
  
  int xCoord = (int) (x*WIDTH_UNIT + (WIDTH_UNIT - PIPE_WIDTH)/2) + X_OFFSET;
  int yCoord = (int) (y*HEIGHT_UNIT) + Y_OFFSET;

  M5.Lcd.fillRect(xCoord, yCoord, PIPE_WIDTH, HEIGHT_UNIT/2, GREEN);
}

void drawEast(int x, int y) {
  
  int xCoord = (int) ((x)*WIDTH_UNIT + WIDTH_UNIT/2) + X_OFFSET;
  int yCoord = (int) (y*HEIGHT_UNIT + (HEIGHT_UNIT - PIPE_WIDTH)/2) + Y_OFFSET;

  M5.Lcd.fillRect(xCoord, yCoord, WIDTH_UNIT/2, PIPE_WIDTH, GREEN);
}

void drawSouth(int x, int y) {
  
  int xCoord = (int) (x*WIDTH_UNIT + (WIDTH_UNIT - PIPE_WIDTH)/2) + X_OFFSET;
  int yCoord = (int) (y*HEIGHT_UNIT) + HEIGHT_UNIT/2 + Y_OFFSET;

  M5.Lcd.fillRect(xCoord, yCoord, PIPE_WIDTH, HEIGHT_UNIT/2, GREEN);
}

void drawWest(int x, int y) {
  
  int xCoord = (int) (x*WIDTH_UNIT) + X_OFFSET;
  int yCoord = (int) (y*HEIGHT_UNIT + (HEIGHT_UNIT - PIPE_WIDTH)/2)+ Y_OFFSET;

  M5.Lcd.fillRect(xCoord, yCoord, WIDTH_UNIT/2, PIPE_WIDTH, GREEN);
}
