#include <SoftwareSerial.h>  //Librairie Software Serial Port  
#define RX 10                //Pin 10 pour RX 
#define TX 11                //Pin 11 pour TX

// Creation objet serie pour le hc06
SoftwareSerial PhmHC06Serie(RX,TX);  

String cmd=""; 
const int led = 12;

void setup()  
{  
  pinMode(led, OUTPUT);
  Serial.begin(9600);    
  Serial.println("Pret ...");  
  PhmHC06Serie.begin(9600);  //Valeur par défaut pour le HC06
}  
void loop()  
{  
 //Lecture des données HC06
  while(PhmHC06Serie.available()>0){
    cmd+=(char)PhmHC06Serie.read();
    delayMicroseconds(500);
    Serial.println(cmd);
  }

  //Traitement de la commande recue
  if(cmd!=""){
    Serial.print("Une commande a ete recue ....");
    Serial.println(cmd);
    // Commande LEDON pour allumage LED
    if(cmd=="LEDON"){
        Serial.println("Allumage LED");
        digitalWrite(led, LOW);   
    }
    // Commande LEDOFF pour extinction LED
    if(cmd=="LEDOFF"){
        Serial.println("Extinction LED");
                digitalWrite(led, HIGH); 
    }
    cmd=""; //Reset variable commande envoyée par Bluetooth
  }
}
