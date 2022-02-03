
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>


//temps à determiner
const long finAlarme = 60000; //permet de déterminer le temps durant lequel l'alarme doit sonner sans interruption
const int tempsPause = 100; //temps de pause entre chaque augmentation de luminosité de la bande LED
const int tempsSnooze = 5400;  //temps de report de la musique pdt le snooze


//définition des variables LED

const int led = 10; //LED pin 10
int X = 25; //nombre de led sur la bande
int j;  //pour régler l'intensité des leds
int lum = 255; //permet de régler l'intensité maximale des LED

Adafruit_NeoPixel strip = Adafruit_NeoPixel(X, led, NEO_GRB + NEO_KHZ800);

#define RX 12
#define TX 13

SoftwareSerial mySerial(RX, TX);

//Définition des variables et commandes pour le système sonore

//VARIABLES
static int8_t Send_buf[8] = {0} ;//The MP3 player undestands orders in a 8 int string

int16_t musique = 0x0006; // numéro musique dans la carte SD (ici la numéro 6)

//COMMANDES
//all the commands needed in the datasheet(http://geekmatic.in.ua/pdf/Catalex_MP3_board.pdf)

#define CMD_SEL_DEV 0X09 //SELECT STORAGE DEVICE, DATA IS REQUIRED 
#define DEV_TF 0X02 //HELLO,IM THE DATA REQUIRED  

#define STOP_PLAY 0X16 //commande arrêtant la musique
#define SINGLE_PLAY 0X08//commande permettant de jouer un son en boucle

//fonction permettant d'écrire les commandes à envoyer au module cataplex (son)

void sendCommand(int8_t command, int16_t dat) {
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for (uint8_t i = 0; i < 8; i++) {
    mySerial.write(Send_buf[i]) ;
  }
}


// définition des variables et fonction pour bouton arrêt et capteur vibration

const int tilt = 2;  //pin du capteur de vibrations NE PAS CHANGER
const int boutonArret = 4; //pin du bouton d'arrêt
int valBoutonArret = 0; //permet de stocker l'état du bouton d'arrêt
unsigned long difference = 0;
int interruption = 0; //permet de compter les fois ou le capteur est enclenché
unsigned long debutSnooze = 0; //permet stockage du moment où débute le snooze
unsigned long tempsCourant = 0;
long pdt = 0;

void onEvent() { //ce qu'il se passe lorsque le capteur vibre
  interruption += 1;
  Serial.print("vibration : ");
  Serial.println(interruption);
}




//autres variables
int alarme = 2; // code état alarme enclenchée (1), snooze (0), pas débutée (2), arrêtée definitivement (3)
long debut; // permet de stocker le moment de début de l'alarme





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //pinMode
  pinMode(tilt, INPUT_PULLUP);
  pinMode(boutonArret, INPUT);
  pinMode(led, OUTPUT);

  //setup capteur
  attachInterrupt(digitalPinToInterrupt(tilt), onEvent, CHANGE);
  Serial.println(F("Système intialisé"));

  //setup bande LED
  strip.begin();
  strip.show();

  //setup HP
  mySerial.begin(9600);//Start our Serial coms for our serial monitor!
  delay(500);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card
  delay(200);


  // augmentation de la luminosité petit à petit :

  Serial.println("début allumage LED");

  for (j = 1; j <= lum; j++) {

    //si on stoppe l'alarme alors que la lumière s'allume (son non-commencé)
    if (digitalRead(boutonArret) == LOW) {
      Serial.println("annulation de l'alarme (avant début musique mais pendant allumage LED)");
      strip.setBrightness(0);                                                           //on éteint la lumière
      strip.show();
      alarme = 3;
      j = lum;
      Serial.print("état de l'alarme (enclenchée (1), snooze (0), pas débutée (2), arrêtée definitivement (3)) : "); Serial.println(alarme);
    }

    else {
      strip.setBrightness(j);  // réglage de la luminosité
      for (uint16_t i = 0; i < X; i++) {    // boucle permettant d'allumer chaque LED
        strip.setPixelColor(i, 245, 128, 0); //couleur choisie pour imiter le jaune du soleil
      }
      strip.show();
      delay(tempsPause); //pemet de ne pas augmenter la luminosité trop rapidement
      Serial.print("niveau de luminosité : "); Serial.println(j);
    }
  }
  //fin augmentation luminosité bande LED (qui reste ensuite constante jusqu'à l'arrêt complet de l'alarme)


  // début de l'alarme
  interruption = 0; // on réinitialise à 0 le nombre de vibrations du capteur
  if (alarme != 3) {
    Serial.println("déclenchement alarme (son)");
    sendCommand(SINGLE_PLAY, musique);//La chanson va jouer en boucle mais sans empecher la carte de fonctionner
    debut = millis(); //on marque le début de l'alarme
    alarme = 1;   // l'alarme est enclenchée
    Serial.print("état de l'alarme (enclenchée (1), snooze (0), pas débutée (2), arrêtée (3)) : "); Serial.println(alarme);
  }

  //Alarme sonore

  //tant que l'alarme n'a pas été arrêtée, ni ne sonne depuis plus de x temps (x est à determiner grâce à finAlarme)
  while (((pdt - debut) < finAlarme) && (alarme != 3)) {

    valBoutonArret = digitalRead(boutonArret); //lecture état du bouton d'arrêt

    //si on arrête l'alarme alors qu'elle sonne
    if (valBoutonArret == LOW) {
      alarme = 3;
      sendCommand(STOP_PLAY, 0x0000); //on arrête la musique
      strip.setBrightness(0);  //on éteint la bande LED
      strip.show();
      Serial.println("arrêt mécanique alarme (pendant qu'elle sonne)");
      Serial.print("état de l'alarme (enclenchée (1), snooze (0), pas débutée (2), arrêtée (3)) : "); Serial.println(alarme);
    }

    pdt = millis();  //on marque le moment actuel (pour ensuite savoir depuis cmb de temps l'alarme sonne)

    // si l'alarme sonne
    if (alarme == 1) {

      // si le capteur vibre == snooze enclenché
      if (interruption != 0) {

        debutSnooze = millis();   //on marque le début du temps de snooze
        Serial.println("début du snooze");
        sendCommand(STOP_PLAY, 0x0000);  //on arrête la musique
        alarme = 0;
        Serial.print("état de l'alarme (enclenchée (1), snooze (0), pas débutée (2), arrêtée (3)) : "); Serial.println(alarme);

        //tant que la limite de temps du snooze n'a pas été atteinte
        while (difference < tempsSnooze) {
          tempsCourant = millis(); //on marque le moment actuel pour ensuite savoir depuis cmb de temps snooze a été enclenché)
          difference = tempsCourant - debutSnooze;

          valBoutonArret = digitalRead(boutonArret); // lecture du bouton d'arrêt

          //si on arrête l'alarme alors que l'on a déjà enclenché snooze
          if (valBoutonArret == LOW) {
            alarme = 3;
            sendCommand(STOP_PLAY, 0x0000); //on arrête la musique
            difference = tempsSnooze;  //permet d'arrêter le décompte du snooze
            strip.setBrightness(0);  //on éteint la bande LED
            strip.show();
            Serial.println("arrêt mécanique alarme (pendant le snooze)");
            Serial.print("état de l'alarme (enclenchée (1), snooze (0), pas débutée (2), arrêtée (3)) : "); Serial.println(alarme);
          }

        }
        Serial.println("fin du snooze, réenclenchement de l'alarme");
        difference = 0; //réinitialisation du temps de snooze
        interruption = 0; //réinitialisation du capteur

        //si l'alarme n'a pas été arrêtée pdt le snooze
        if (alarme != 3) {
          alarme = 1;
          Serial.print("état de l'alarme (enclenchée (1), snooze (0), pas débutée (2), arrêtée (3)) : "); Serial.println(alarme);
          sendCommand(SINGLE_PLAY, musique);//La chanson va jouer en boucle mais sans empecher la carte de fonctionner
          debut = millis(); //on marque le début de l'alarme (depuis son réenclenchement)
          pdt = 0; //réinitialisation du chiffre permettant de calculer la différence entre le moment de début de l'alarme et le moment courant
        }

      } //fin du snooze
    }
  }

  if (alarme == 1) { //(sert simplement aux commentaires, normalement tout les autres cas on été traités)
    strip.setBrightness(0); //extinction de la bande LED
    strip.show();
    sendCommand(STOP_PLAY, 0x0000); //arrêt du son
    alarme = 3;
    Serial.print("état de l'alarme (enclenchée (1), snooze (0), pas débutée (2), arrêtée (3)) : "); Serial.println(alarme);
    Serial.println("arrêt automatique de l'alarme, temps dépassé");
  }
  else if (alarme != 3) {
    Serial.print("état de l'alarme (enclenchée (1), snooze (0), pas débutée (2), arrêtée (3)) : "); Serial.println(alarme);
    Serial.println("ISSUE ANORMALE");
  }
  Serial.println("fin du setup");
}



void loop() {
  // put your main code here, to run repeatedly:

}
