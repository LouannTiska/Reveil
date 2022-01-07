#include <Adafruit_NeoPixel.h>

int X=25; //nombre de led sur la bande
int j=0;  //pour régler l'intensité des leds

Adafruit_NeoPixel strip = Adafruit_NeoPixel(X, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.show();
    
  for(j=0; j<=100; j++){strip.setBrightness(j); 
    for(uint16_t i=0; i<X; i++){strip.setPixelColor(i,245, 128, 0);}
    delay(200);
    strip.show();
}
}

void loop() {
  // put your main code here, to run repeatedly:

}
