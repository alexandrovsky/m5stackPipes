#include <M5Stack.h>
#include "utility/MPU9250.h"
#include "Pipe.h"

MPU9250 IMU;
int xRot, yRot, zRot;
long oldTime, curTime;

int WIDTH = 5;
int HEIGHT = 5;
int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 240;

PipesGameFiled gameField;

void setup()
{
  M5.begin();
  Wire.begin();

  IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);
  IMU.initMPU9250();

  gameField = new GameFiled(WIDTH, HEIGHT);
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
  
  for(int x=0; x <= WIDTH; x++) 
  {
    int tempX = (int) (x*(float)(SCREEN_WIDTH)/gameField.width);
    M5.Lcd.drawLine(tempX, 0, tempX, SCREEN_HEIGHT, RED);
  }

  for(int y=0; y <= HEIGHT; y++) 
  {
    int tempY = (int) (y*(float)(SCREEN_HEIGHT)/gameField.height);
    M5.Lcd.drawLine(0, tempY, SCREEN_WIDTH, tempY, RED);
  }
}

void drawPipe(int x, int y) {
  
}
