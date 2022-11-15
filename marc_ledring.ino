/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-web-server-slider-pwm/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#define PIN        5 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 25 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long hexToDec(String hexString) {
  
  unsigned long decValue = 0;
  long nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = long(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
  return decValue;
}

// Replace with your network credentials
const char* ssid = "nemeth_wifi";
const char* password = "75000000";

const int output = 14;
const int output2 = 12;
const int output3 = 15;

String sliderValue = "0";

String sliderValue2 = "0";

String sliderValue3 = "0";

String sliderValue4 = "#000000";
String ipCim = WiFi.localIP().toString();

const char* PARAM_INPUT = "value";
const char* PARAM_INPUT2 = "value2";
const char* PARAM_INPUT3 = "value3";
const char* PARAM_INPUT4 = "value4";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Web Server</title>
  <style>
    html {
  font-family: Arial; 
  display: inline-block; 
  text-align: center;
  }
  
    h2 {
  font-size: 2.3rem;
  }
  
    p {
  font-size: 1.9rem;
  }
  
    body {
  max-width: 400px;
  margin:0px auto;
  padding-bottom: 25px;
  }
  
  .picker{
  width: 100px; 
  height: 100px;
  }
    .slider { 
  -webkit-appearance: none; 
  margin: 14px; 
  width: 360px; 
  height: 25px;
  background: #FFD65C;
    outline: none; 
  -webkit-transition: .2s; 
  transition: opacity .2s;
  }
    .slider::-webkit-slider-thumb {
  -webkit-appearance: none; 
  appearance: none; 
  width: 35px; 
  height: 35px; 
  background: #003249; 
  cursor: pointer;
  }
  
    .slider::-moz-range-thumb { 
  width: 35px;
  height: 35px;
  background: #003249;
  cursor: pointer; 
  }
    pwmSlider4
    {
    color = red;
    }
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><span id="textSliderValue2">%SLIDERVALUE2%</span></p>
  <p><span id="textSliderValue3">%SLIDERVALUE3%</span></p>
   <p><span id="textSliderValue4">%SLIDERVALUE4%</span></p>
   <p><span id="IPCIM">%IPCIM%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="1023" value="%SLIDERVALUE%" step="1" class="slider"></p>
  <p><input type="range" onchange="updateSliderPWM2(this)" id="pwmSlider2" min="0" max="1023" value="%SLIDERVALUE2%" step="1" class="slider"></p>
  <p><input type="range" onchange="updateSliderPWM3(this)" id="pwmSlider3" min="0" max="1023" value="%SLIDERVALUE3%" step="1" class="slider"></p>
  <p><input type="color" onchange="updateSliderPWM4(this)" id="pwmSlider4"  value="%SLIDERVALUE4%" class="picker"></p>
<script>

function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
function updateSliderPWM2(element) {
  var sliderValue2 = document.getElementById("pwmSlider2").value;
  document.getElementById("textSliderValue2").innerHTML = sliderValue2;
  console.log(sliderValue2);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value2="+sliderValue2, true);
  xhr.send();
}
function updateSliderPWM3(element) {
  var sliderValue3 = document.getElementById("pwmSlider3").value;
  document.getElementById("textSliderValue3").innerHTML = sliderValue3;
  console.log(sliderValue3);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value3="+sliderValue3, true);
  xhr.send();
}
function updateSliderPWM4(element) {
  var sliderValue4 = document.getElementById("pwmSlider4").value;
  document.getElementById("textSliderValue4").innerHTML = sliderValue4;
  console.log(sliderValue4);
  var csakszin =sliderValue4[1]+sliderValue4[2]+sliderValue4[3]+sliderValue4[4]+sliderValue4[5]+sliderValue4[6] ;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value4="+csakszin, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  if (var == "IPCIM"){
    return ipCim;
  }
  if (var == "SLIDERVALUE2"){
    return sliderValue2;
  }
  if (var == "SLIDERVALUE3"){
    return sliderValue3;
  }
  if (var == "SLIDERVALUE4"){
    return sliderValue4;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(output,OUTPUT);
  pinMode(output2,OUTPUT);
  pinMode(output3,OUTPUT);

 pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  ipCim = WiFi.localIP().toString();
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      Serial.println("egy: "+inputMessage);
      analogWrite(output, sliderValue.toInt());
    }
    if (request->hasParam(PARAM_INPUT2)) {
      inputMessage = request->getParam(PARAM_INPUT2)->value();
      sliderValue2 = inputMessage;
      Serial.println("ketto: "+inputMessage);
      analogWrite(output2, sliderValue2.toInt());
    }
    if (request->hasParam(PARAM_INPUT3)) {
      inputMessage = request->getParam(PARAM_INPUT3)->value();
      sliderValue3 = inputMessage;
      Serial.println("harom: "+inputMessage);
      analogWrite(output3, sliderValue3.toInt());
    }
    if (request->hasParam(PARAM_INPUT4)) {
      inputMessage = request->getParam(PARAM_INPUT4)->value();
      sliderValue4 = inputMessage;
      Serial.print("negy: ");
      Serial.println(inputMessage);
      long szin =hexToDec(sliderValue4);
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(((szin>>16)&0b11111111), ((szin>>8)&0b11111111), (szin&0b11111111)));
      }
    pixels.show();   // Send the updated pixel colors to the hardware.
    }
    else {
      inputMessage = "No message sent";
      Serial.println(inputMessage);
    }
    
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
}
