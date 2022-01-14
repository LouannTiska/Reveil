#include <Adafruit_NeoPixel.h>

const int led = 10; //LED pin 10
int X = 25; //nombre de led sur la bande
int j;  //pour régler l'intensité des leds
int lum = 255; //permet de régler l'intensité maximale des LED
const int tempsPause = 100; //temps de pause entre chaque augmentation de luminosité

Adafruit_NeoPixel strip = Adafruit_NeoPixel(X, led, NEO_GRB + NEO_KHZ800);

// SNOOZE
int valled = 0; //permet de stocker la valeur qui passe par la pin de la LED
int valbouton = 0; //permet de stocker la valeur qui passe par la pin du bouton SNOOZE
const int bouton = 9; //pin du bouton SNOOZE
const int boutonArret = 11; // pin du bouton d'arrêt
unsigned long difference = 0; //initialisation de la variable qui va stocker le temps depuis le début du snooze
unsigned long tempsSnooze = 9000; //temps durant lequel on arrête la musique du reveil après avoir appuyé sur snooze
unsigned long debutSnooze = 0;
unsigned long tempsCourant = 0;

//MUSIQUE 35secondes (à jouer 9 fois pour arriver à environ 5min)
const int buzzer = 8; //pin du buzzer
int etatMusique = 0; //1 si la musique est en train de jouer, 0 sinon
const int repetMusique = 3; //combien de tour fait la musique avant de s'eteindre
int compteurMusique = 1; //sert à compter combien de fois joue la musique


// Change to 0.5 for a slower version of the song, 1.25 for a faster version (1 being the normal version)
const float songSpeed = 1.0;

// Defining frequency of each music note
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

// Music notes of the song, 0 is a rest/pulse
int notes[] = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_D5, NOTE_E5, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
  NOTE_C5, NOTE_A4, NOTE_B4, 0,

  NOTE_A4, NOTE_A4,
  //Repeat of first part
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_D5, NOTE_E5, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
  NOTE_C5, NOTE_A4, NOTE_B4, 0,
  //End of Repeat

  NOTE_E5, 0, 0, NOTE_F5, 0, 0,
  NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
  NOTE_D5, 0, 0, NOTE_C5, 0, 0,
  NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

  NOTE_E5, 0, 0, NOTE_F5, 0, 0,
  NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
  NOTE_D5, 0, 0, NOTE_C5, 0, 0,
  NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4
};

// Durations (in ms) of each music note of the song
// Quarter Note is 250 ms when songSpeed = 1.0
int durations[] = {
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125,
  //Rpeat of First Part
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,
  //End of Repeat

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500,

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500
};

void setup() {
  // put your setup code here, to run once:
  //LED
  Serial.begin(9600);
  strip.begin();
  strip.show();

  for (j =1; j <= lum; j++) {
    if (digitalRead(boutonArret) == LOW) {
      compteurMusique = repetMusique;
      lum = 0;
      strip.setBrightness(0);
      strip.show();
    }else {

    strip.setBrightness(j);
    for (uint16_t i = 0; i < X; i++) {
      strip.setPixelColor(i, 245, 128, 0);
    }
    strip.show();
    delay(tempsPause); Serial.print("niveau de luminosité : "); Serial.println(j);
  }}
  //SNOOZE
  pinMode(bouton, INPUT);
  pinMode(boutonArret, INPUT);

  //MUSIQUE
  const int totalNotes = sizeof(notes) / sizeof(int);
  int u = 0;
  while (compteurMusique < repetMusique) {
    // Loop through each note
    compteurMusique = compteurMusique + 1;
    etatMusique = 1;
    for (int i = u; i < totalNotes; i++) {
      if (digitalRead(boutonArret) == LOW) {
        Serial.println("arrêt2");
        compteurMusique = repetMusique;
        i = totalNotes;
        strip.setBrightness(0);
        strip.show();
      }
      Serial.print("note "); Serial.println(i);

      if (etatMusique == 1) {
        valbouton = digitalRead(bouton);
        if (valbouton == LOW) {
          debutSnooze = millis();
          Serial.print("début du snooze");
          while (difference < tempsSnooze) {
            noTone(buzzer);
            i = 0;
            etatMusique = 0;
            tempsCourant = millis();
            difference = tempsCourant - debutSnooze;
            Serial.println(difference);
          }
          difference = 0;
          etatMusique = 1;
          compteurMusique = 0;
        }
      }

      const int currentNote = notes[i];
      float wait = durations[i] / songSpeed;
      // Play tone if currentNote is not 0 frequency, otherwise pause (noTone)
      if (currentNote != 0)
      {
        tone(buzzer, notes[i], wait); // tone(pin, frequency, duration)
      }
      else
      {
        noTone(buzzer);
      }
      // delay is used to wait for tone to finish playing before moving to next loop
      delay(wait);
    } Serial.print("nombre de repetition de la musique : "); Serial.println(compteurMusique);
  } Serial.println("fin du setup");
}


void loop() {
  // put your main code here, to run repeatedly:
}
