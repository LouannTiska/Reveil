// Interrupteur
int valled = 0;
int valbouton = 0;
const int led = 2;
const int bouton = 7;
unsigned long difference = 0;
const int tempsSnooze = 5400;
unsigned long debutSnooze = 0;
unsigned long tempsCourant = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(led, OUTPUT);
pinMode(bouton, INPUT);
digitalWrite(led, LOW);
delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  valled = digitalRead(led);
if (valled == LOW) {
  valbouton = digitalRead(bouton);
  if (valbouton == LOW) {
    debutSnooze = millis();
    Serial.print("difference de temps : début du snooze :");
    Serial.println(tempsCourant - debutSnooze);
    while (difference < tempsSnooze) {
      digitalWrite(led, HIGH);
      tempsCourant = millis();
      difference = tempsCourant - debutSnooze;
      Serial.print("différence : ");
      Serial.println(tempsCourant - debutSnooze);}
    digitalWrite(led, LOW);
    difference = 0;
  }
  
}



}
