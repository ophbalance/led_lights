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
      input[type=range] {
  -webkit-appearance: none;
  margin: 18px 0;
  width: 100%;
}
input[type=range]:focus {
  outline: none;
}
input[type=range]::-webkit-slider-runnable-track {
  width: 75%;
  height: 8.4px;
  cursor: pointer;
  animate: 0.2s;
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  background: #3071a9;
  border-radius: 1.3px;
  border: 0.2px solid #010101;
}
input[type=range]::-webkit-slider-thumb {
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  border: 1px solid #000000;
  height: 36px;
  width: 16px;
  border-radius: 3px;
  background: #ffffff;
  cursor: pointer;
  -webkit-appearance: none;
  margin-top: -14px;
}
input[type=range]:focus::-webkit-slider-runnable-track {
  background: #367ebd;
}
input[type=range]::-moz-range-track {
  width: 100%;
  height: 8.4px;
  cursor: pointer;
  animate: 0.2s;
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  background: #3071a9;
  border-radius: 1.3px;
  border: 0.2px solid #010101;
}
input[type=range]::-moz-range-thumb {
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  border: 1px solid #000000;
  height: 36px;
  width: 16px;
  border-radius: 3px;
  background: #ffffff;
  cursor: pointer;
}
input[type=range]::-ms-track {
  width: 100%;
  height: 8.4px;
  cursor: pointer;
  animate: 0.2s;
  background: transparent;
  border-color: transparent;
  border-width: 16px 0;
  color: transparent;
}
input[type=range]::-ms-fill-lower {
  background: #2a6495;
  border: 0.2px solid #010101;
  border-radius: 2.6px;
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
}
input[type=range]::-ms-fill-upper {
  background: #3071a9;
  border: 0.2px solid #010101;
  border-radius: 2.6px;
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
}
input[type=range]::-ms-thumb {
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
  border: 1px solid #000000;
  height: 36px;
  width: 16px;
  border-radius: 3px;
  background: #ffffff;
  cursor: pointer;
}
input[type=range]:focus::-ms-fill-lower {
  background: #3071a9;
}
input[type=range]:focus::-ms-fill-upper {
  background: #367ebd;
}
    </style>
    <script>
    function sendPosition(slider, value) {
      console.log(slider+": "+value);
    
      var request = new XMLHttpRequest();
      request.open("POST", "http://10.10.1.1/"); // Add the right URI
      request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
      request.send(encodeURI(slider)+"="+value);
    }
    function sendText(txt, value) {
      console.log(txt+": "+value);
    
      var request = new XMLHttpRequest();
      request.open("POST", "http://10.10.1.1/"); // Add the right URI
      request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
      request.send(encodeURI(txt)+"="+value);
    }
    </script>
  </head>
  <body>
    <h1>ESP8266 Wi-Fi Access Point and Web Server Demo</h1>
    <p><a href='/?led=1' class='buttonred'>Red Alliance</a> <a href='/?led=2' class='buttonblue'>Blue Alliance</a> <a href='/?led=3' class='buttongreen'>Green Alliance</a></p>
    <p><input id="bright" type="range" min=0 max=255 onchange="sendPosition(this.id, this.value);"></p>
    <form action="" method="get" name="meh">
    <p><input id="formtxt" name="formtxt" value="" ></p>
    <p><input type="submit" id="submit" name="submit" value="submit"></p>
    
  </body>
</html>
)=====";
