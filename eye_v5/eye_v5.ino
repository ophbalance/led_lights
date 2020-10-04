//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 8
#define EYE D2
#define EYE2 D3

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
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, EYE,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(8, 8, EYE2,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

uint32_t RED1 = matrix.Color(0,0,255);
uint32_t RED2 = matrix2.Color(255,0,0);
  
void setup() {
  // put your setup code here, to run once:
  matrix.begin();  
  matrix.setBrightness(5);
  matrix2.begin();  
  matrix2.setBrightness(5);
  Serial.begin(115200);
  Serial.println();
  Serial.println("start");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //random along display
  int setX = random(0);
  int setY = random(1,7);
  int randColor = random(1,5);
//  int randEffect = random(1,5);
  
  matrix.fillScreen(RED1);
  matrix.show();
  Serial.println("first matrix");
  matrix2.fillScreen(RED2);
  matrix2.show();
  Serial.println("second matrix");
  delay(5500);
}
