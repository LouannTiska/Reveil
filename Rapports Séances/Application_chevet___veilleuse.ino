// Bluetooth
#include <SoftwareSerial.h>  //Librairie Software Serial Port  
#define RX 10                //Pin 10 pour RX 
#define TX 11                //Pin 11 pour TX

// Creation objet serie pour le hc06
SoftwareSerial PhmHC06Serie(RX,TX); 
 
String cmd="";

//Bande LED
#include <Adafruit_NeoPixel.h>
// Lumière d'ambiance
int X = 12;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(X, 6, NEO_GRB + NEO_KHZ800);
// Veilleuse
int Y = 25;
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(Y, 5, NEO_GRB + NEO_KHZ800);
 

void setup() {  
  //Bluetooth
  Serial.begin(9600);    
  Serial.println("Pret ...");  
  PhmHC06Serie.begin(9600);  //Valeur par défaut pour le HC06

  //Bande LED
  strip.begin();
  strip.show();
  strip2.begin();
  strip2.show();
}  
void loop()  
{  
 //Lecture des données HC06
  while(PhmHC06Serie.available()>0){
    cmd+=(char)PhmHC06Serie.read();
    delayMicroseconds(50);
    Serial.println(cmd);
  }

  //Traitement de la commande recue
  if(cmd!=""){
    Serial.print("Une commande a ete recue ....");
    Serial.println(cmd);
    // Commande couleur pour allumage LED
        Serial.println("Allumage LED");
        if(cmd=="jaune"){
        for(int i = 0; i < X; i++ ) {strip.setPixelColor(i, 245, 128, 0);}
        strip.show();
        Serial.println("Lumière jaune"); 
        delay(1000);
        }
        if(cmd=="rouge"){
        for(int i = 0; i < X; i++ ) {strip.setPixelColor(i, 223, 0, 0);}
        strip.show();
        Serial.println("Lumière rouge"); 
        delay(1000);
        }
        if(cmd=="violet"){
        for(int i = 0; i < X; i++ ) {strip.setPixelColor(i, 102, 0, 204);}
        strip.show();
        Serial.println("Lumière violette"); 
        delay(1000);
        } 
        if(cmd=="bleu"){
        for(int i = 0; i < X; i++ ) {strip.setPixelColor(i, 0, 0, 223);}
        strip.show();
        Serial.println("Lumière bleu"); 
        delay(1000);
        }
        if(cmd=="veilleuse"){
        for(int i = 0; i < Y; i++ ) {strip2.setPixelColor(i, 255, 255, 255);}
        strip2.show();
        Serial.println("Veilleuse"); 
        delay(1000);
        }
    }
    // Commande LEDOFF pour extinction LED
    if(cmd=="LEDOFF"){
        Serial.println("Extinction LED");
        for(int i = 0; i < X; i++ ) {strip.setPixelColor(i, 0, 0, 0);}
        strip.show();
        Serial.println("Lumière jaune"); 
        delay(1000); 
    }
    if(cmd=="LEDOFF Veilleuse"){
        Serial.println("Extinction LED");
        for(int i = 0; i < Y; i++ ) {strip2.setPixelColor(i, 0, 0, 0);}
        strip2.show();
        Serial.println("Lumière jaune"); 
        delay(1000); 
    }
    cmd=""; //Reset variable commande envoyée par Bluetooth
  }
