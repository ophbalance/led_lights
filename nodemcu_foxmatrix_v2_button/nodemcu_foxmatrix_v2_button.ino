/* Create a WiFi access point and provide a web server on it. 
** For more details see http://42bots.com.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/Org_01.h>
#include "index.h"
#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 21
#define PIN 14
#define BUTTON_PIN 13
#define PASS 1

// Setting up some loop variables here
int mywidth = MATRIX_WIDTH;
int pass = 1;
int myloop = 1;
int caseloop = 1;
int brightLevel=100;
String passTxt="";
int buttonState = 0;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int lastButtonState = LOW;   // the previous reading from the input pin


/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "6004-galaxy-hat";
const char *password = "yomomma2";

//Matrix setup
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(21, 6, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

//Colors array
const uint16_t colors[] = {
  matrix.Color(0, 0, 0),matrix.Color(255, 0, 0), matrix.Color(0, 0, 255), matrix.Color(0, 255, 0), matrix.Color(255, 255, 255) };

//Moving vox
const unsigned char fox1[MATRIX_HEIGHT][MATRIX_WIDTH]={
    {0, 0, 1, 0, 0 , 0 , 0, 1, 0, 0, 3, 3, 3, 3, 0, 2, 0, 0, 0, 2, 0},
    {0, 1, 2, 1, 1 , 1 , 1, 2, 1, 0, 3, 0, 0, 0, 2, 0, 1, 0, 1, 0, 2},
    {1, 2, 2, 0, 0 , 0 , 0, 2, 2, 1, 3, 0, 0, 0, 2, 0, 0, 1, 0, 0, 2},
    {0, 1, 3, 3, 0 , 0 , 3, 3, 1, 0, 3, 3, 3, 0, 2, 0, 1, 0, 1, 0, 2},
    {0, 0, 1, 0, 0 , 0 , 0, 1, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
    {0, 0, 0, 1, 2 , 2 , 1, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0}
};

//Fox for changing brightness (reverse every other row)
const unsigned char flashyfox[MATRIX_HEIGHT][MATRIX_WIDTH]={
    {0, 0, 1, 0, 0 , 0 , 0, 1, 0, 0, 1, 1, 1, 1, 0, 2, 0, 0, 0, 2, 0},
    {2, 0, 1, 0, 1 , 0 , 2, 0, 0, 0, 1, 0, 0, 1, 2, 1, 1, 1, 1, 2, 1},    
    {1, 2, 2, 0, 0 , 0 , 0, 2, 2, 1, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 2},
    {2, 0, 1, 0, 1 , 0 , 2, 0, 1, 1, 1, 0, 0, 1, 4, 4, 0, 0, 4, 4, 1},    
    {0, 0, 1, 0, 0 , 0 , 0, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
    {0, 2, 0, 0, 0 , 2 , 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0}
};

// Define a web server at port 80 for HTTP




void setup() {
  matrix.setFont(&Org_01);
  matrix.begin();  
  matrix.setTextWrap(false);
  matrix.setBrightness(100);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}



// Loop that runs until we turn off the device
void loop() {
  


  switch (caseloop) {
  case 1:
    drawStillFox();Serial.println("case1");
    break;
  case 2:
    drawText(pass,passTxt);Serial.println("case2");
    break;
  case 3:
    drawFox();Serial.println("case3");
    break;
  case 4:
    drawText(pass,passTxt);Serial.println("case4");
    break;
  case 5:
    drawFlashyFox();Serial.println("case5");
    break;
  case 6:
    drawText(pass,passTxt);Serial.println("case6");
    break;
  default:
    // if nothing else matches, do the default
    // default is optional
    break;
  }

  //how many times we're in here
  caseloop++;
  myloop++;
  //pass++;
  if(caseloop >= 6) caseloop = 1;
}

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
        pass++;
        if(pass>3){pass=1;}
      }
    }    
  }
  lastButtonState = reading;
}

void drawFox(){
  matrix.fillScreen(0);
  matrix.show();
  delay(300);
  for (unsigned char i=0; i<34;i++){
  for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
    for(unsigned char y=0; y<MATRIX_WIDTH; y++) {      
      matrix.drawPixel((y-22)+i, x, colors[fox1[x][y]]);      
      readPinState();  
    }
  } matrix.show();
  delay(100);
  }
}

void drawStillFox(){
  matrix.fillScreen(0);
  matrix.show();
  delay(300);
  
  for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
    for(unsigned char y=0; y<MATRIX_WIDTH; y++) {      
      matrix.drawPixel(y, x, colors[fox1[x][y]]);      
      readPinState();  
    }
  } matrix.show();
  delay(10000);  
}

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
        readPinState();  
      }
      
    } delay(60);
    matrix.show();
  }   
  //matrix.setBrightness(brightLevel);
  //delay(300);
}

void drawText(int passme, String passtxt) {
  int looper=150;
  matrix.setTextColor(colors[passme]);
  for(int f=0;f<looper;f++){
    matrix.fillScreen(0);
    matrix.setCursor(mywidth, 4);
    if(--mywidth < -100) {
      mywidth = matrix.width();
    }    

    if(passtxt!=""){
      matrix.print((passtxt));
      readPinState();  
    } else if(passme==1){
      matrix.print(F("6004 RED ALLIANCE"));
      readPinState();  
    } else if (passme==2) {
      matrix.print(F("6004 BLUE ALLIANCE"));
      readPinState();  
    } else if (passme==3){
      matrix.print(F("6004 GREEN ALLIANCE"));
      readPinState();  
    }
    
    
    matrix.show();
    delay(100);
  }
  
}
