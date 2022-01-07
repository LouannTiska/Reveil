#include <LiquidCrystal.h>

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

LiquidCrystal lcd(2,3,4,5,6,7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
  
  lcd.begin(16,2);
  //lcd.print("Bonjour");
  //lcd.setCursor(0,1);
  //lcd.print("je suis en peip2");
}

void loop() {
  // put your main code here, to run repeatedly:
tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();

    lcd.setCursor(4,0);
    lcd.print(tm.Day);
    lcd.print('/');
    lcd.print(tm.Month);
    lcd.print('/');
    lcd.print(tmYearToCalendar(tm.Year));
    lcd.print(' ');
    lcd.setCursor(6,1);
    lcd.print(tm.Hour);
    lcd.print(':');
    lcd.print(tm.Minute);
    
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
