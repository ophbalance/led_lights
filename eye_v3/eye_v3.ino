#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 32
#define PIN D2
#define BUTTON_PIN 14
#define PASS 1

int brightLevel=15;

//Matrix setup
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

//Colors array
//blank, orange, red, blue, white
const uint16_t colors[] = {
  matrix.Color(0, 0, 0), matrix.Color(255, 165, 0), matrix.Color(255, 0, 0), matrix.Color(0, 0, 255), matrix.Color(0, 255, 0), matrix.Color(255, 255, 255) };

// Bitmaps
static const uint8_t PROGMEM blink_1[8] = {B01111110, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B01111110};
static const uint8_t PROGMEM blink_2[8] = {B00000000, B00000000, B00111100, B01111110, B01111110, B00111100, B00000000, B00000000};
static const uint8_t PROGMEM blink_3[8] = {B00000000, B00000000, B00000000, B01111110, B01111110, B00000000, B00000000, B00000000};
static const uint8_t PROGMEM blink_4[8] = {B00000000, B00000000, B00000000, B00000000, B01111110, B00000000, B00000000, B00000000};
static const uint8_t PROGMEM blink_5[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

void setup() {
  // put your setup code here, to run once:
  matrix.begin();  
  matrix.setTextWrap(false);
  matrix.setBrightness(25);
  Serial.begin(115200);
  Serial.println();
  Serial.println("start");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //random along display
  int setX = random(0,24);
  int setY = random(1,7);
 

  drawEye(setX, setY);
  drawBlink(setX,0);
 
}

void drawEye(int x, int y){
  
  
  //random amount to look around
  int randLook = random(3,8);

  //draw eye  
  for(int i=0;i<=randLook;i++){
    //random x/y for pupil
    int randX=random(x-2,x+5);
    int randY=random(3,5);
    //fill blank and redrew with pupil
    matrix.fillScreen(0);
    matrix.drawBitmap(x, 0, blink_1, 8, 8, colors[4]);
    matrix.fillRect(randX+2, randY, 3, 3, colors[0]);  
    matrix.show();
    delay(1000);
  }
}

void drawBlink(int x, int y){
  
  //draw closing blink
  int looper = 5;
  for(int f=2;f<=looper;f++){
    matrix.fillScreen(0);
    switch (f) {
      case 2:
        matrix.drawBitmap(x, y, blink_2, 8, 8, colors[4]);
        break;
     case 3:
        matrix.drawBitmap(x, y, blink_3, 8, 8, colors[4]);
        break;
     case 4:
        matrix.drawBitmap(x, y, blink_4, 8, 8, colors[4]);
        break;
     case 5:
        matrix.drawBitmap(x, y, blink_5, 8, 8, colors[4]);
        break;
    }    
    matrix.show();
    delay(50);
  }
  matrix.fillScreen(0);
  matrix.show();
  //draw open blink
  looper = 1;
  for(int f=5;f>=looper;f--){
    matrix.fillScreen(0);
    switch (f) {
      case 5:
        matrix.drawBitmap(x, y, blink_5, 8, 8, colors[4]);
        break;
     case 4:
        matrix.drawBitmap(x, y, blink_4, 8, 8, colors[4]);
        break;
     case 3:
        matrix.drawBitmap(x, y, blink_3, 8, 8, colors[4]);
        break;
     case 2:
        matrix.drawBitmap(x, y, blink_2, 8, 8, colors[4]);
        break;
     case 1:
        matrix.drawBitmap(x, y, blink_1, 8, 8, colors[4]);
        break;
    }    
    matrix.show();
    delay(50);
  }
  matrix.fillScreen(0);
  matrix.show();
}
