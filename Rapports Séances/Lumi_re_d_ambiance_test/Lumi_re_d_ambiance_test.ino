#include <Adafruit_NeoPixel.h>

int X = 25;
String couleur = "violet"; 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(X, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
 if(couleur=="rouge"){
 for(int i = 0; i < X; i++ ) {strip.setPixelColor(i, 223, 0, 0);}
    strip.show(); 
    delay(1000);
}
if(couleur=="violet"){
 for(int i = 0; i < X; i++ ) {strip.setPixelColor(i, 116, 101, 188);}
    strip.show(); 
    delay(1000);
}
if(couleur=="blue"){
 for(int i = 0; i < X; i++ ) {strip.setPixelColor(i, 0, 0, 223);}
    strip.show(); 
    delay(1000);
}
}
