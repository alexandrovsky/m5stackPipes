//initialize variables
int xRotOld, yRotOld, zRotOld;


//rotate 
  //to the right
  if (zRotOld > zRot) 
  {
    //RotateRight();
        M5.Lcd.setCursor(0, 160); M5.Lcd.print("to the right ->");
  }
  else if (zRotOld < zRot)
  {
        M5.Lcd.setCursor(0, 160); M5.Lcd.print("to the left <-");
  }

//tilt up and down 
  //down to up
  if (xRotOld > xRot) {
    M5.Lcd.setCursor(0, 160); M5.Lcd.print("up ^");
  }
  else if (xRotOld < xRot){
    M5.Lcd.setCursor(0, 160); M5.Lcd.print("down v");
 }  

  //tilt from right to left
  //right up
  if (yRotOld > yRot) {
        M5.Lcd.setCursor(0, 160); M5.Lcd.print("up right <");
  }
  else if (yRotOld < yRot){
        M5.Lcd.setCursor(0, 160); M5.Lcd.print("up left >");
  }


  //button A pressed
  if (M5.BtnA.read()){
    M5.Lcd.setCursor(0, 160); M5.Lcd.print("A is pressed");
  }

  //button B pressed
  if (M5.BtnB.read()){
    M5.Lcd.setCursor(0, 160); M5.Lcd.print("B is pressed");
  }

  //button C pressed
  if (M5.BtnC.read()){
    M5.Lcd.setCursor(0, 160); M5.Lcd.print("C is pressed");
  }


  xRotOld = xRot;
  yRotOld = yRot; 
  zRotOld = zRot;


//array bauen mit n werten
//wenn neuer wert, alle werte einen nach hintne
//neuer wert hinten anfügen

//durchs array durcheghen durchschnitt berechnen, -> den fürs event nehmen

//über die länge des array bestimmt wie sehr man glättet
//erst mal 3-6 werte 

int smoothArray[SMOOTH_LEN] = {0};
int smoothIt;

#define SMOOTH_LEN 5
  public smooth(int[] smoothArray, int value) {
    for(int i = 1, i < SMOOTH_LEN, i++) {
      smoothArray[i-1] = smoothArray[i];
      if (i == SMOOTH_LEN-1) {
        smoothArray[i] == value; 
      }
    }
    
    for(int i = 0, i < SMOOTH_LEN, i++) {
      smoothIt += smoothArray[i];
    }
    smoothIt = smoothIt/SMOOTH_LEN;
  }
