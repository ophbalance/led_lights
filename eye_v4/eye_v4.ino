//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 8
#define PIN D2
#define PIN2 D3
#define BUTTON_PIN D4
#define PASS 1

int brightLevel=15;
int pass = 1;
int mainLoop=1;

// button bounce
int buttonState = 0;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int lastButtonState = LOW;   // the previous reading from the input pin

//Matrix setup
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(8, 8, PIN2,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
//Colors array
//blank, orange, red, blue, white
const uint16_t colors[] = {
  matrix.Color(0, 0, 0), matrix.Color(255, 165, 0), matrix.Color(255, 0, 0), matrix.Color(0, 0, 255), matrix.Color(0, 255, 0), matrix.Color(255, 255, 255) };
const uint16_t colors2[] = {
  matrix2.Color(0, 0, 0), matrix2.Color(255, 165, 0), matrix2.Color(255, 0, 0), matrix2.Color(0, 0, 255), matrix2.Color(0, 255, 0), matrix2.Color(255, 255, 255) };

// Bitmaps
// blink
static const uint8_t PROGMEM blink_1[8] = {B01111110, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B01111110};
static const uint8_t PROGMEM blink_2[8] = {B00000000, B00000000, B00111100, B01111110, B01111110, B00111100, B00000000, B00000000};
static const uint8_t PROGMEM blink_3[8] = {B00000000, B00000000, B00000000, B01111110, B01111110, B00000000, B00000000, B00000000};
static const uint8_t PROGMEM blink_4[8] = {B00000000, B00000000, B00000000, B00000000, B01111110, B00000000, B00000000, B00000000};
static const uint8_t PROGMEM blink_5[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};
// Angry eye
static const uint8_t PROGMEM angry_left[8] = {
  B10000000, 
  B11100000, 
  B11110000, 
  B11111000, 
  B11111100, 
  B11111110, 
  B11111111, 
  B01111110
  };
static const uint8_t PROGMEM angry_right[8] = {
  B00000001, 
  B00000111, 
  B00001111, 
  B00011111, 
  B00111111, 
  B01111111, 
  B11111111, 
  B01111110
  };  
// Happy
static const uint8_t PROGMEM happy1[8] = {
  B01111110, 
  B11111111, 
  B11111111, 
  B11111111, 
  B11100111, 
  B11000011, 
  B10000001, 
  B00000000
  };  
  
void setup() {
  // put your setup code here, to run once:
  matrix.begin();  
  matrix.setTextWrap(false);
  matrix.setBrightness(5);
  matrix2.begin();  
  matrix2.setTextWrap(false);
  matrix2.setBrightness(5);
  Serial.begin(115200);
  Serial.println();
  Serial.println("start");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  //random along display
  int setX = random(0);
  int setY = random(1,7);
  int randColor = random(1,5);
//  int randEffect = random(1,5);
  
  switch (mainLoop){
    case 1: 
      //drawEye(setX, setY, randColor);
      //drawBlink(setX,0, randColor);
      Serial.println("case1");
      
      delay(1000); 
      drawSingleEye(setX, randColor);
      
      break;
    case 2:
      //drawAngry(setX, randColor);
      //drawBlink(setX,0, randColor);
      Serial.println("case2");
      break;      
    case 3:
      //brakeLight();
      //delay(2000);
      //remBrakeLight();
      Serial.println("case3");
      break;
    case 4:
      //drawHappy(setX, randColor);
      //drawBlink(setX,0, randColor);
      Serial.println("case4");
      break;
  }
  mainLoop++;
  if(mainLoop>4){mainLoop=1;}
 
}

// Draw Happy Eye
void drawHappy(int x, int color){
  matrix.fillScreen(0);
  matrix.drawBitmap(x, 0, happy1, 8, 8, colors[color]);
  matrix.drawBitmap(x+10, 0, happy1, 8, 8, colors[color]);
  matrix.show();
  delay(5000);
}

// Draw Angry Eye
void drawAngry(int x, int color){
  matrix.fillScreen(0);
  matrix.drawBitmap(x, 0, angry_left, 8, 8, colors[color]);
  matrix.drawBitmap(x+10, 0, angry_right, 8, 8, colors[color]);
  matrix.show();
  delay(5000);
}
// Draw the eye(s).  If sent random x/y will bounce around matrix
/*void drawEye(int x, int y, int color){
  
  
  //random amount to look around
  int randLook = random(3,8);

  //draw eye 1
  for(int i=0;i<=randLook;i++){
    //random x/y for pupil
    int randX=random(x-2,x+5);
    int randY=random(3,5);
    //fill blank and redrew with pupil
    matrix.fillScreen(0);
    matrix.drawBitmap(x, 0, blink_1, 8, 8, colors[color]);
    matrix.drawBitmap(x+10, 0, blink_1, 8, 8, colors[color]);
    matrix.fillRect(randX+2, randY, 3, 3, colors[0]);  
    matrix.fillRect(randX+2+10, randY, 3, 3, colors[0]);  
    matrix.show();
    delay(1500);
  }
}*/

// Draw the eye(s).  If sent random x/y will bounce around matrix
void drawSingleEye(int x, int color){
  //random amount to look around
  int randLook = random(3,8);
  int myx = 3;
  int myy = 3;
  //draw eye 1
  for(int i=0;i<=randLook;i++){
    //random x/y for pupil
    int randX=random(myx-2,myx+5);
    Serial.println("X:");
    Serial.println(randX);
    int randY=random(myy-2,myy+5);
    Serial.println("Y:");
    Serial.println(randY);
    //fill blank and redrew with pupil
    matrix.drawBitmap(0, 0, blink_1, 8, 8, colors[color]);
    matrix.fillRect(randX, randY, 3, 3, colors[0]);  
    matrix.show();
    matrix2.fillScreen(0);
    matrix2.drawBitmap(0, 0, blink_1, 8, 8, colors2[color]);
    matrix2.fillRect(randX, randY, 3, 3, colors2[0]);  
    matrix.show();
    delay(5500);
  }
}

// Draw the eye(s) blinking.  If sent random x/y will bounce around matrix
void drawBlink(int x, int y, int color){
  
  //draw closing blink
  int looper = 5;
  for(int f=2;f<=looper;f++){
    matrix.fillScreen(0);
    switch (f) {
      case 2:
        matrix.drawBitmap(x, y, blink_2, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_2, 8, 8, colors[color]);
        break;
     case 3:
        matrix.drawBitmap(x, y, blink_3, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_3, 8, 8, colors[color]);
        break;
     case 4:
        matrix.drawBitmap(x, y, blink_4, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_4, 8, 8, colors[color]);
        break;
     case 5:
        matrix.drawBitmap(x, y, blink_5, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_5, 8, 8, colors[color]);
        break;
    }    
    matrix.show();
    delay(50);
  }
  matrix.fillScreen(1);
  matrix.show();
  //draw open blink
  looper = 1;
  for(int f=5;f>=looper;f--){
    matrix.fillScreen(0);
    switch (f) {
      case 5:
        matrix.drawBitmap(x, y, blink_5, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_5, 8, 8, colors[color]);
        break;
     case 4:
        matrix.drawBitmap(x, y, blink_4, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_4, 8, 8, colors[color]);
        break;
     case 3:
        matrix.drawBitmap(x, y, blink_3, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_3, 8, 8, colors[color]);
        break;
     case 2:
        matrix.drawBitmap(x, y, blink_2, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_2, 8, 8, colors[color]);
        break;
     case 1:
        matrix.drawBitmap(x, y, blink_1, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_1, 8, 8, colors[color]);
        break;
    }    
    matrix.show();
    delay(50);
  }
  matrix.fillScreen(0);
  matrix.show();
}

// Draw the eye(s) blinking.  If sent random x/y will bounce around matrix
void drawSingleBlink(int x, int y, int color){
  
  //draw closing blink
  int looper = 5;
  for(int f=2;f<=looper;f++){
    matrix.fillScreen(0);
    switch (f) {
      case 2:
        matrix.drawBitmap(x, y, blink_2, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_2, 8, 8, colors[color]);
        break;
     case 3:
        matrix.drawBitmap(x, y, blink_3, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_3, 8, 8, colors[color]);
        break;
     case 4:
        matrix.drawBitmap(x, y, blink_4, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_4, 8, 8, colors[color]);
        break;
     case 5:
        matrix.drawBitmap(x, y, blink_5, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_5, 8, 8, colors[color]);
        break;
    }    
    matrix.show();
    delay(50);
  }
  matrix.fillScreen(1);
  matrix.show();
  //draw open blink
  looper = 1;
  for(int f=5;f>=looper;f--){
    matrix.fillScreen(0);
    switch (f) {
      case 5:
        matrix.drawBitmap(x, y, blink_5, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_5, 8, 8, colors[color]);
        break;
     case 4:
        matrix.drawBitmap(x, y, blink_4, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_4, 8, 8, colors[color]);
        break;
     case 3:
        matrix.drawBitmap(x, y, blink_3, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_3, 8, 8, colors[color]);
        break;
     case 2:
        matrix.drawBitmap(x, y, blink_2, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_2, 8, 8, colors[color]);
        break;
     case 1:
        matrix.drawBitmap(x, y, blink_1, 8, 8, colors[color]);
        matrix.drawBitmap(x+10, y, blink_1, 8, 8, colors[color]);
        break;
    }    
    matrix.show();
    delay(50);
  }
  matrix.fillScreen(0);
  matrix.show();
}

void brakeLight(){
  matrix.setBrightness(200);
  matrix.fillScreen(colors[2]);
  matrix.show();
}

void remBrakeLight(){
  matrix.setBrightness(25);
  matrix.fillScreen(0);
  matrix.show();
}

// Read button for stop light
void readPinState(){
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        brakeLight();
      } else {
        remBrakeLight();
      }
    }    
  }
  lastButtonState = reading;
}
