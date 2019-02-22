#include <M5Stack.h>
#include "utility/MPU9250.h"
#include "Pipe.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define RADIUS 5
#define PIPE_WIDTH 10

float WIDTH_UNIT = (float)(SCREEN_WIDTH)/GRID_WIDTH;
float HEIGHT_UNIT = (float)(SCREEN_HEIGHT)/GRID_HEIGHT;


MPU9250 IMU;
int xRot, yRot, zRot;
long oldTime, curTime;

PipesGameField gameField;

void setup()
{
  M5.begin();
  Wire.begin();

  IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);
  IMU.initMPU9250();
}

void loop() 
{
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
    
    draw();
  }
}

void draw()
{
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
  
  for(int x=0; x <= GRID_WIDTH; x++) 
  {
    int tempX = (int) (x*(float)(SCREEN_WIDTH)/GRID_WIDTH);
    M5.Lcd.drawLine(tempX, 0, tempX, SCREEN_HEIGHT, RED);
  }

  for(int y=0; y <= GRID_HEIGHT; y++) 
  {
    int tempY = (int) (y*(float)(SCREEN_HEIGHT)/GRID_HEIGHT);
    M5.Lcd.drawLine(0, tempY, SCREEN_WIDTH, tempY, RED);
  }

  for(int x=0; x < GRID_WIDTH; x++) {
    for(int y=0; y < GRID_HEIGHT; y++) {
      drawPipe(x, y);
    }
  }
}

void drawPipe(int x, int y) {

  Pipe p = gameField.getPipe(x, y);

  drawConnection(x,y,p);

  int xCoord = (int) (x*WIDTH_UNIT + WIDTH_UNIT/2 - RADIUS);
  int yCoord = (int) (y*HEIGHT_UNIT + HEIGHT_UNIT/2 - RADIUS);

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
        case 0:
          drawNorth(x,y);
          break;
        case 1:
          drawEast(x,y);
          break;
        case 2:
          drawWest(x,y);
          break;
        case 3:
          drawSouth(x,y);
          break;
      }
    }
  }
}

void drawNorth(int x, int y) {
  
  int xCoord = (int) (x*WIDTH_UNIT - PIPE_WIDTH);
  int yCoord = (int) (y*HEIGHT_UNIT - PIPE_WIDTH);

  M5.Lcd.fillRect(xCoord, yCoord, PIPE_WIDTH, WIDTH_UNIT/2, GREEN);
}

void drawEast(int x, int y) {
  
  int xCoord = (int) ((x+1)*WIDTH_UNIT - PIPE_WIDTH);
  int yCoord = (int) (y*HEIGHT_UNIT - PIPE_WIDTH);

  M5.Lcd.fillRect(xCoord, yCoord, WIDTH_UNIT/2, PIPE_WIDTH, GREEN);
}

void drawSouth(int x, int y) {
  
  int xCoord = (int) (x*WIDTH_UNIT - PIPE_WIDTH);
  int yCoord = (int) ((y+1)*HEIGHT_UNIT - PIPE_WIDTH);

  M5.Lcd.fillRect(xCoord, yCoord, PIPE_WIDTH, -WIDTH_UNIT/2, GREEN);
}

void drawWest(int x, int y) {
  
  int xCoord = (int) (x*WIDTH_UNIT - PIPE_WIDTH);
  int yCoord = (int) (y*HEIGHT_UNIT - PIPE_WIDTH);

  M5.Lcd.fillRect(xCoord, yCoord, WIDTH_UNIT/2, PIPE_WIDTH, GREEN);
}
