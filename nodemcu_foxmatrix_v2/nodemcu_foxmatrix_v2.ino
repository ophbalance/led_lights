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
#define PIN 12
#define PASS 1

// Setting up some loop variables here
int mywidth = MATRIX_WIDTH;
int pass = 1;
int myloop = 1;
int caseloop = 1;
int brightLevel=100;
String passTxt="";

IPAddress    apIP(10, 10, 1, 1);  // Defining a static IP address: local & gateway
                                    // Default IP in AP mode is 192.168.4.1

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
ESP8266WebServer server(80);


void handleRoot() {


  char ledText[80];
  if (server.arg("led")==""){pass=1;}else{pass = server.arg("led").toInt();}
  if (server.arg("bright")==""){brightLevel=200;}else{brightLevel = server.arg("bright").toInt();}
  if (server.arg("formtxt")==""){passTxt="";}else{passTxt = String(server.arg("formtxt"));}
  
  
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;


  String html = MAIN_page;
  server.send ( 200, "text/html", html );
  digitalWrite ( LED_BUILTIN, 1 );
}

void handleNotFound() {
  //digitalWrite ( LED_BUILTIN, 0 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

void setup() {
  matrix.setFont(&Org_01);
  matrix.begin();  
  matrix.setTextWrap(false);
  matrix.setBrightness(5);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
  server.on ( "/", handleRoot );
  server.on ( "/led=3", handleRoot);
  server.on ( "/led=2", handleRoot);
  server.on ( "/led=1", handleRoot);  
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );
  server.onNotFound ( handleNotFound );
  
  server.begin();
  Serial.println("HTTP server started");
}



// Loop that runs until we turn off the device
void loop() {
  server.handleClient();

  // Max bright every 5th loop
  //if (myloop%5 ==0){
  //  matrix.setBrightness(5);
  //} else {
  //  matrix.setBrightness(brightLevel);
  //}

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


void drawFox(){
  matrix.fillScreen(0);
  matrix.show();
  delay(300);
  for (unsigned char i=0; i<34;i++){
  for(unsigned char x=0; x<MATRIX_HEIGHT; x++) {
    for(unsigned char y=0; y<MATRIX_WIDTH; y++) {      
      matrix.drawPixel((y-22)+i, x, colors[fox1[x][y]]);      
    }
  } matrix.show();server.handleClient();
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
    }
  } matrix.show();server.handleClient();
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
      }
      
    } delay(60);
    matrix.show();server.handleClient();
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
    } else if(passme==1){
      matrix.print(F("6004 RED ALLIANCE"));
    } else if (passme==2) {
      matrix.print(F("6004 BLUE ALLIANCE"));
    } else if (passme==3){
      matrix.print(F("6004 GREEN ALLIANCE"));
    }
    
    
    matrix.show();server.handleClient();
    delay(100);
  }
  
}
