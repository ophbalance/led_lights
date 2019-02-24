const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <title>ESP8266 WiFi Network</title>
    <style>
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; font-size: 1.5em; Color: #000000; }
      h1 { Color: #AA0000; }
      .buttongreen {  background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 24px;} 
      .buttonblue {  background-color: #008CBA; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 24px;} 
      .buttonred {  background-color: #f44336; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 24px;} 
      .buttongrey {  background-color: #555555; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 24px;} 
    </style>
  </head>
  <body>
    <h1>ESP8266 Wi-Fi Access Point and Web Server Demo</h1>
    <p><a href='/?led=1' class='buttonred'>Red Alliance</a> <a href='/?led=2' class='buttonblue'>Blue Alliance</a> <a href='/?led=3' class='buttongreen'>Green Alliance</a></p>
  </body>
</html>
)=====";
