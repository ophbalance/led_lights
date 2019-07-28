/* Create a WiFi access point and provide a web server on it. 
** For more details see http://42bots.com.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
//#include <Fonts/Org_01.h>
#include "index.h"
#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 32
#define PIN D2
#define BUTTON_PIN 14
#define PASS 1

// Setting up some loop variables here
int mywidth = MATRIX_WIDTH;
int pass = 1;
int myloop = 1;
int caseloop = 1;
int brightLevel=5;
String passTxt="";
int buttonState = 0;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int lastButtonState = LOW;   // the previous reading from the input pin

//IPAddress    apIP(10, 10, 1, 1);  // Defining a static IP address: local & gateway
                                    // Default IP in AP mode is 192.168.4.1

/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "6004-pit";
const char *password = "yomomma2";

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
static const uint8_t PROGMEM blink_1[8] = {B00000000, B00111100, B01111110, B01111110, B01111110, B01111110, B00111100, B00000000};
static const uint8_t PROGMEM blink_2[8] = {B00000000, B00000000, B00111100, B01111110, B01111110, B00111100, B00000000, B00000000};
static const uint8_t PROGMEM blink_3[8] = {B00000000, B00000000, B00000000, B01111110, B01111110, B00000000, B00000000, B00000000};
static const uint8_t PROGMEM blink_4[8] = {B00000000, B00000000, B00000000, B00000000, B01111110, B00000000, B00000000, B00000000};
static const uint8_t PROGMEM blink_5[8] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

// Animations
static const uint8_t *blinkImg[]={blink_1, blink_2, blink_3, blink_4, blink_5};
int wait = 50; // In milliseconds
int second_in_ms = 1000;
int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

uint8_t 
  blinkIndex[] = { 0, 1, 2, 3, 4, 3, 2, 1, 0},
  heartIndex = 0,
  heartCounter = 10,
  blinkCountdown = 10,
  gazeCountdown  =  20, // Countdown to next eye movement
  gazeFrames     =  30; // Duration of eye movement (smaller = faster)

int8_t
  eyeX = 3, eyeY = 3,   // Current eye position
  newX = 3, newY = 3,   // Next eye position
  dX   = 0, dY   = 0;   // Distance from prior to new position

//Eyes shifting
int shift_first_eye = 8;
int shift_second_eye = 32;

enum MatrixMode { EYE, TEXT, HEART, COUNTDOWN, CONFUSE  };
enum EyeMode {NORMAL, LOOKING_LEFT, LOOKING_RIGHT, LOOKING_UP, LOOKING_DOWN, CROSS_EYED, RANDOM};
String CSARR = "Vive CSARR !";
String RDY = "GEORGE EST PRET";
String MAMMOUTH = "VISE LE MAMMOUTH";

MatrixMode currentMode = EYE;
EyeMode currentEyeMode = RANDOM;
String currentText = RDY;

int countdown_index = 9;
int confuse_index = 0;
char countdown[10] = {'0','1','2','3','4','5','6','7','8','9'};
uint32_t last_image_timestamp = 0;

//Eyes
const unsigned char eye1[MATRIX_HEIGHT][MATRIX_WIDTH]={
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const unsigned char eye2[MATRIX_HEIGHT][MATRIX_WIDTH]={
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const unsigned char eye3[MATRIX_HEIGHT][MATRIX_WIDTH]={
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

//Moving vox
const unsigned char fox1[MATRIX_HEIGHT][MATRIX_WIDTH]={
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 2, 1, 0, 0, 0, 1, 2, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0, 1, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 0, 2, 2, 0, 0, 0, 0, 1, 1, 3, 0, 0, 0, 3, 1, 1},
    {0, 1, 4, 4, 4, 0, 0, 0, 4, 4, 4, 1, 0, 2, 2, 2, 2, 0, 0, 1, 1, 0, 3, 0, 3, 0, 1, 1},
    {0, 1, 0, 0, 4, 0, 0, 0, 4, 0, 0, 1, 0, 2, 2, 2, 2, 0, 0, 1, 1, 0, 0, 3, 0, 0, 1, 1},
    {0, 0, 1, 0, 0, 3, 3, 3, 0, 0, 1, 0, 0, 2, 2, 0, 0, 0, 0, 1, 1, 0, 3, 0, 3, 0, 1, 1},
    {0, 0, 0, 1, 0, 0, 3, 0, 0, 1, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 3, 1, 0},
    {0, 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}
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
//ESP8266WebServer server(80);


void setup() {
  //matrix.setFont(&Org_01);
  matrix.begin();  
  matrix.setTextWrap(false);
  matrix.setBrightness(15);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  //set-up the custom IP address
  //WiFi.mode(WIFI_AP_STA);
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  
  // You can remove the password parameter if you want the AP to be open. 
  //WiFi.softAP(ssid, password);

  //IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
 

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
}



// Loop that runs until we turn off the device
void loop() {
  //server.handleClient();
  readPinState();
  Serial.println(pass);
  // Max bright every 5th loop
  //if (myloop%5 ==0){
  //  matrix.setBrightness(5);
  //} else {
  //  matrix.setBrightness(brightLevel);
  //}

  switch (caseloop) {
  case 1:
    eye_mode_perform();Serial.println("case1");
    break;
  case 2:
    drawEye2();Serial.println("case2");
    break;
  case 3:
    drawEye3();Serial.println("case3");
    break;
  case 4:
    drawText(pass,passTxt);Serial.println("case4");
    break;
  case 5:
    drawStillFox();Serial.println("case5");
    break;
  case 6:
    passTxt="Go Team 6004!";
    drawText(pass,passTxt);Serial.println("case6");
    passTxt="";
    break;
  default:
    // if nothing else matches, do the default
    // default is optional
    break;
  }

  //how many times we're in here
  caseloop++;
  myloop++;
  pass++;
  if(pass >= 2) pass=1;
  if(caseloop >= 2) caseloop = 1;
}

void eye_mode_perform(){
  if(millis() >= last_image_timestamp + wait){
    matrix.setRotation(2);
    matrix.fillScreen(LOW);
    matrix.drawBitmap(0 + shift_first_eye, 0,
      blinkImg[
        (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
          blinkIndex[blinkCountdown] : 0          // No, show bitmap 0
      ], 8, 8, HIGH);
    matrix.drawBitmap(0 + shift_second_eye, 0,blinkImg[
        (blinkCountdown < sizeof(blinkIndex)) ? // Currently blinking?
          blinkIndex[blinkCountdown] : 0          // No, show bitmap 0
      ], 8, 8, HIGH);
    // Decrement blink counter.  At end, set random time for next blink.
    if(--blinkCountdown == 0) blinkCountdown = random(5, 100);

    if(--gazeCountdown <= gazeFrames) { //--gazeCountdown <= gazeFrames
      // Eyes are in motion - draw pupil at interim position
      matrix.fillRect(
        newX - (dX * gazeCountdown / gazeFrames) + shift_first_eye,
        newY - (dY * gazeCountdown / gazeFrames),
        2, 2, LOW);
      matrix.fillRect(
        newX - (dX * gazeCountdown / gazeFrames) + shift_second_eye,
        newY - (dY * gazeCountdown / gazeFrames),
        2, 2, LOW);
      if(gazeCountdown == 0) {    // Last frame?
        eyeX = newX; eyeY = newY; // Yes.  What's new is old, then...
        do { // Pick random positions until one is within the eye circle
          newX = random(3) + 2; newY = random(3) + 2;
          dX   = newX - 4;  dY   = newY - 4;
        } while((dX * dX + dY * dY) >= 16);      // Thank you Pythagoras
        dX            = newX - eyeX;             // Horizontal distance to move
        dY            = newY - eyeY;             // Vertical distance to move
        gazeCountdown = random(gazeFrames, 60); // Count to end of next movement
      }
    } else {
      // Not in motion yet -- draw pupil at current static position
      matrix.fillRect(eyeX +shift_first_eye, eyeY, 2, 2, LOW);
                        matrix.fillRect(eyeX +shift_second_eye, eyeY, 2, 2, LOW);
    }
    matrix.show();
    last_image_timestamp = millis();
  }
}

void drawEye1(){
  matrix.fillScreen(0);
  matrix.show();
 // delay(300);
  
  for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
    for(unsigned char y=0; y<MATRIX_WIDTH; y++) {      
      matrix.drawPixel(y, x, colors[eye1[x][y]]);    
      readPinState();  
    }
  } matrix.show();
  delay(10000);  
}

void drawEye2(){
  matrix.fillScreen(0);
  matrix.show();
  //delay(300);
  
  for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
    for(unsigned char y=0; y<MATRIX_WIDTH; y++) {      
      matrix.drawPixel(y, x, colors[eye2[x][y]]);    
      readPinState();  
    }
  } matrix.show();
  delay(1000);  
}

void drawEye3(){
  matrix.fillScreen(0);
  matrix.show();
  //delay(300);
  if(millis() >= last_image_timestamp + wait){
   
  }

  /*
  for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
    for(unsigned char y=0; y<MATRIX_WIDTH; y++) {      
      matrix.drawPixel(y, x, colors[eye3[x][y]]);    
      readPinState();  
    }
  } matrix.show();
  */
  //delay(10000);  
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
  for (unsigned char i=0; i<64;i++){
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
  //matrix.setBrightness(5);
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
    matrix.setCursor(mywidth, 0);
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
