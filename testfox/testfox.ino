// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/Org_01.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif
#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 24
#define PIN 6
#define PASS 1

// This is a 24 x grid.  You will need to change the array's below if the size of the grid changes
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(24, 6, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(0, 0, 0),matrix.Color(255, 0, 0), matrix.Color(0, 0, 255), matrix.Color(0, 255, 0), matrix.Color(255, 255, 255) };

const unsigned char fox1[MATRIX_HEIGHT][MATRIX_WIDTH]={
    {0, 0, 0, 1, 0, 0 , 0 , 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 2, 0, 0, 0, 2, 0, 0},
    {0, 0, 1, 2, 1, 1 , 1 , 1, 2, 1, 0, 0, 1, 0, 0, 0, 2, 0, 1, 0, 1, 0, 2, 0},
    {0, 1, 2, 2, 0, 0 , 0 , 0, 2, 2, 1, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 2, 0},
    {0, 0, 1, 3, 3, 0 , 0 , 3, 3, 1, 0, 0, 1, 1, 1, 0, 2, 0, 1, 0, 1, 0, 2, 0},
    {0, 0, 0, 1, 0, 0 , 0 , 0, 1, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0},
    {0, 0, 0, 0, 1, 2 , 2 , 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0}
};

const unsigned char flashyfox[MATRIX_HEIGHT][MATRIX_WIDTH]={
    {0, 0, 0, 1, 0, 0 , 0 , 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 2, 0, 0, 0, 2, 0, 0},
    {0, 2, 0, 1, 0, 1 , 0 , 2, 0, 0, 0, 1, 0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 0, 0},    
    {0, 1, 2, 2, 0, 0 , 0 , 0, 2, 2, 1, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 2, 0},
    {0, 2, 0, 1, 0, 1 , 0 , 2, 0, 1, 1, 1, 0, 0, 1, 4, 4, 0, 0, 4, 4, 1, 0, 0},    
    {0, 0, 0, 1, 0, 0 , 0 , 0, 1, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0},
    {0, 0, 2, 0, 0, 0 , 2 , 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0, 0}
};
const unsigned char eye1[MATRIX_HEIGHT][MATRIX_WIDTH]={
    {0, 0, 1, 1, 1, 1 , 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1 , 1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1 , 1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1 , 1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1 , 1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1 , 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
void setup() {
  matrix.setFont(&Org_01);  //tiny font for tiny display
  matrix.begin();  
  matrix.setTextWrap(false);
  matrix.setBrightness(5);  //very minimal brightness level

}
// Used for looping in various programs
int mywidth = MATRIX_WIDTH;
int pass = 1;
int myloop = 1;
int brightLevel=5;


void loop() {

  //Every 5 loops got o max brightness
  if (myloop%5 ==0){
    matrix.setBrightness(255);
  } else {
    matrix.setBrightness(brightLevel);
  }
  
  drawStillFox();
  drawText(pass);
  drawFox();
  drawFlashyFox();
  myloop++;
  pass++;
  if(pass >= 4) pass = 1;
}

//Moving fox
void drawFox(){
  matrix.fillScreen(0);
  matrix.show();
  delay(300);
  for (unsigned char i=0; i<34;i++){
  for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
    for(unsigned char y=0; y<MATRIX_WIDTH; y++) {      
      matrix.drawPixel((y-22)+i, x, colors[fox1[x][y]]);      
    }
  } matrix.show();
  delay(100);
  }
}

//Non-moving fox
void drawStillFox(){
  matrix.fillScreen(0);
  matrix.show();
  delay(300);
  
  for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
    for(unsigned char y=0; y<MATRIX_WIDTH; y++) {      
      matrix.drawPixel(y, x, colors[fox1[x][y]]);      
    }
  } matrix.show();
  delay(10000);  
}

//Fox that changes brightness levels.  Requires reversing odd rows to draw the right pixels
void drawFlashyFox(){
  matrix.fillScreen(0);
  matrix.show();
  matrix.setBrightness(50);
  for(unsigned char i=0;i<45;i++){
   int mycnt=0;
    for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
      for(unsigned char y=0; y<MATRIX_WIDTH; y++) {
        if(flashyfox[x][y] == 0){
          matrix.setPixelColor(mycnt,0,0,0);
        } else if (flashyfox[x][y] == 1) {
          matrix.setPixelColor(mycnt,random(1,50),0,0);
        } else if (flashyfox[x][y] == 2) {
          matrix.setPixelColor(mycnt,0,random(1,50),0);
        } else if (flashyfox[x][y] == 3) {
          matrix.setPixelColor(mycnt,0,0,random(1,50));
        } else if (flashyfox[x][y] == 4) {
          matrix.setPixelColor(mycnt,random(1,255),random(1,255),random(1,255));
        }
        mycnt++;
      }
      
    } delay(60);
    matrix.show();
  }   
  matrix.setBrightness(brightLevel);
  //delay(300);
}

// Draw the text only
void drawText(int passme) {
  int looper=150;
  matrix.setTextColor(colors[passme]);
  for(int f=0;f<looper;f++){
    matrix.fillScreen(0);
    matrix.setCursor(mywidth, 4);
    if(--mywidth < -100) {
      mywidth = matrix.width();
    }    
    
    if(passme==1){
      matrix.print(F("6004 RED ALLIANCE"));
    } else if (passme==2) {
      matrix.print(F("6004 BLUE ALLIANCE"));
    } else if (passme==3){
      matrix.print(F("6004 GREEN ALLIANCE"));
    }
    
    
    matrix.show();
    delay(100);
  }
  
}
