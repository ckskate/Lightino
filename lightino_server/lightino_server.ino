#include "LPD8806.h"
#include "SPI.h"
#include "Bridge.h"
#include "BridgeServer.h"
#include "BridgeClient.h"


int dataPin = 2;
int clockPin = 3;

BridgeServer server;
LPD8806 strip = LPD8806(32, dataPin, clockPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  
  digitalWrite(13, HIGH);
  Bridge.begin();
  Bridge.put("on", "false");
  strip.begin();
  digitalWrite(13, LOW);

  server.listenOnLocalhost();
  server.begin();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  BridgeClient client = server.accept();

  if (client) {
    process(client);
    client.stop();
  }

  delay(50);
}

void process(BridgeClient client) {
  String command = client.readString();
  command.trim();

    if (command.substring(0, 3) == "off") {
      Bridge.put("on", "false");
      setStripColor(0, 0, 0);
    } 
    else if (command.substring(0, 2) == "on") {
      Bridge.put("on", "true");
      colorCommand(client, command.substring(3));
    } 
    else if (command.substring(0, 3) == "set") {
      Bridge.put("on", "true");
      String test = command.substring(4);
      String red = test.substring(0, 3);
      int r = convertToInt(red);
      Bridge.put("red", String(r));
      String green = test.substring(4, 7);
      int g = convertToInt(green);
      Bridge.put("green", String(g));
      String blue = test.substring(8, 11);
      int b = convertToInt(blue);
      Bridge.put("blue", String(b));
      setStripColor(r, g, b);
    }
    else {
      client.println("Could not process command: " + command);
    }

}

int convertToInt(String ofInterest) {
  if (ofInterest.substring(0, 1).equals("0")) {
    String toReturn = ofInterest.substring(1);
    return toReturn.toInt();
  }
  return ofInterest.toInt();
}

void colorCommand(BridgeClient client, String color) {
  
  if (color == "white") {
    setStripColor(127, 127, 127);
  }
  else if (color == "red") {
    setStripColor(127, 0, 0);
  }   
  else if (color == "yellow") {
    setStripColor(127, 127, 0);
  }  
  else if (color == "green") {
    setStripColor(0, 127, 0);
  }    
  else if (color == "lightblue") {
    setStripColor(0, 127, 127);
  }   
  else if (color == "blue") {
    setStripColor(0, 0, 127);
  } 
  else if (color == "purple") {
    setStripColor(127, 0, 127);
  }
  else if (color == "pink") {
    setStripColor(255, 174, 185);
  }
  else if (color == "rainbow") {
    rainbowCycle(0);
  }
  else if (color == "police") {
    police(0);
  }
  else {
    client.println("Couldn't process your command: " + color);
  }
}

void setStripColor(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 50; j++) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

void police(uint8_t wait) {
  uint16_t i, j, k;

  for (k = 0; k < 500; k++) {
    if (k % 2 == 0) {
      for (j = 0; j < strip.numPixels(); j++) {
        if (j < (strip.numPixels() / 2)) {
          if (j < (strip.numPixels() / 4)) {
            strip.setPixelColor(j, strip.Color(127, 0, 0));
          } else {
            if (j + 2 >= strip.numPixels()/2) {
              strip.setPixelColor(j, strip.Color(127, 127, 127));
            } else {
              strip.setPixelColor(j, 0);
            }
          }    
        } else {
          if (j - strip.numPixels() / 2 < strip.numPixels() / 4) {
            if (j - 2 <= strip.numPixels()/2) {
              strip.setPixelColor(j, strip.Color(127,127,127));
            } else {
              strip.setPixelColor(j, 0);
            }
          } else {
            strip.setPixelColor(j, strip.Color(0, 0, 127));
          }
        }
      }
      strip.show();
    } else {
      for (i = 0; i < strip.numPixels(); i++) {
        if (i < (strip.numPixels() / 2)) {  
          if (i < (strip.numPixels() / 4)) {
            if (i < 2) {
              strip.setPixelColor(i, strip.Color(127,127,127));
            } else {
              strip.setPixelColor(i, 0);
            }
          } else {
            strip.setPixelColor(i, strip.Color(127, 0, 0));
          }
        } else {
          if (i - strip.numPixels() / 2 < strip.numPixels() / 4) {
            strip.setPixelColor(i, strip.Color(0, 0, 127));
          } else {
            if (i + 2 >= strip.numPixels()) {
              strip.setPixelColor(i, strip.Color(127,127,127));
            } else {
              strip.setPixelColor(i, 0);
            }
          }
        }
      }
      strip.show();
    }
    delay(200);
  }
}

uint32_t Wheel(uint16_t WheelPos) {
  byte r, g, b;
  
  switch (WheelPos / 128) {
    case 0:
      r = 127 - WheelPos % 128;
      g = WheelPos % 128;
      b = 0;
      break;
    case 1:
      g = 127 - WheelPos % 128;
      b = WheelPos % 128;
      r = 0;
      break;
    case 2:
      b = 127 - WheelPos % 128;
      r = WheelPos % 128;
      g = 0;
      break;
  }
  return(strip.Color(r,g,b));
}

