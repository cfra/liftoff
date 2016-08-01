//Author Tor

//Die beiden Taster green und red starten die funktionen up und down.
//Die safty funktion stellt sicher, dass sich der Aufzug nicht bewegt und bricht den letzten Befehl ab.
//Es gibt 2 Status LED's rot und gruen. Auf gruenn ist der Aufzug betriebsbereit, auf rot ist eine der beiden Klappen geoeffnet.
//Die Pins 2 und 3 muessen am Relais angeschlossen werden. Sie steuern den Motor.
//P Sind die Signale an den Tuermen, T sind die Taster, S fuer die Safty Klappen
//LOW ist der Zustand 1
//HIGH ist der Zustand 0
//Die Relais haben eine Schaltzeit von maximal 250ms, dies gilt zu beachten sollte man direkt von hoch auf runter schalten.
//Schaltet jeweils 2 Relais fuer hoch oder runter und die Relais fuer L1 und L2.

#include <EEPROM.h>


//      pinName     pinNumber

#define down        2
#define downSR      3

#define up          5
#define upSR        4


#define L1          6
#define L2          7

#define Lock        8
#define Lamp        9

#define P1          24
#define P2A         25
#define P2B         26 // not in use
#define P3          27
#define P4          28

#define green       30
#define red         31
#define blue        32


#define S1          36
#define S2          37

#define N1          40

#define ledR        10
#define ledG        11

#define buttonPressed(a) (digitalRead(a))
#define signalRead(a) (digitalRead(a))

char data = ' '; //Serieller char


//Timervariablen
int startTime =    0;
int currentTime =  0;
int saftyStauts =  0;
int counter = EEPROM.read(0);
byte overRun = EEPROM.read(1); //Error durch Zeitausloesung
byte P4activated = 0;

void setup() {

  Serial.begin(9600); //Uebertragung mit 9600kBits
  Serial.println(EEPROM.read(1)); // Fehlerspeicher

  pinMode(up,       OUTPUT); //Aufzug faehrt hoch
  pinMode(upSR,     OUTPUT);
  pinMode(down,     OUTPUT); //Aufzug faehrt runter
  pinMode(downSR,   OUTPUT);

  pinMode(L1,       OUTPUT); // L1
  pinMode(L2,       OUTPUT); // L2 mitschalten!

  pinMode(Lock,     OUTPUT); // Magnetschloss
  pinMode(Lamp,     OUTPUT); // Lampe

  pinMode(P1,       INPUT); //Signal oben
  pinMode(P2A,      INPUT); //Signal mitte a
  pinMode(P2B,      INPUT); //Signal mitte b
  pinMode(P3,       INPUT); //Signal unten
  pinMode(P4,       INPUT); //Signal Turm 2

  pinMode(green,    INPUT); //Taster gruen
  pinMode(red,      INPUT); //Taster rot
  pinMode(blue,     INPUT); //Taster blau

  pinMode(S1,       INPUT); //Klappe oben
  pinMode(S2,       INPUT); //Klappe unten

  pinMode(N1,       INPUT); //Notaus ueberwachung

  pinMode(ledR,     OUTPUT); //LED
  pinMode(ledG,     OUTPUT); //LED


  //Relais werden auf HIGH geschaltet
  digitalWrite(up,        HIGH);
  digitalWrite(upSR,      HIGH);
  digitalWrite(down,      HIGH);
  digitalWrite(downSR,    HIGH);
  digitalWrite(Lock,      HIGH);
  digitalWrite(Lamp,      HIGH);

  //umschaltung von L1 & L2 auf LOW ( spannung fällt ab)
  digitalWrite(L1,        LOW);
  digitalWrite(L2,        LOW);

  //interner pullUp resitor fuer alle read pins
  digitalWrite(P1,        HIGH);
  digitalWrite(P2A,       HIGH);
  digitalWrite(P2B,       HIGH);
  digitalWrite(P3,        HIGH);
  digitalWrite(P4,        HIGH);

  digitalWrite(green,     HIGH);
  digitalWrite(red,       HIGH);
  digitalWrite(blue,      HIGH);

  digitalWrite(S1,        HIGH);
  digitalWrite(S2,        HIGH);
  digitalWrite(N1,        HIGH);

  digitalWrite(ledR,      LOW);
  digitalWrite(ledG,      LOW);

  //Start sequenz

  blinkGreen(5, 300);

  //Nachdem das System gestartet wurde, blinkt die rote LED 5 mal.
  //Wird green und red gedrueckt, bevor die gruene LED blinkt, startet der Manuelle modus "mLoop()"
  //er kann nur durch neustart verlassen werden
  if ((buttonPressed(green) == 0) &&  buttonPressed(red) == 0) {
    digitalWrite(ledR,    HIGH);
    digitalWrite(ledG,    HIGH);
    delay(1200);
    digitalWrite(ledR,    LOW);
    mLoop();
  }
  blinkRed(5, 200);
}

//Main
void loop() {
  //Die klappen werden immer ueberwacht
  safty();

  //fahert von unten nach oben entlaed und fahert wieder runter
  if (buttonPressed(green) == 0 && signalRead(P4) == 1 && safty()) {
    delay(120); // Knopf muss xms gedrückt sein
    if (buttonPressed(green) == 0 && signalRead(P4) == 1 && safty()) {
      startTime = millis(); //Laufzeit berechnung
      bring(16000);
      delay(3000); // Kiste rutscht vom Aufzug -- hoffentlich!
      startTime = millis(); //time reset
      back(16000);
    }
  } else if (signalRead(P4) == 0) {
    blinkGreen(4 , 150); //zeigt an, dass eine Kiste oben entladen werden muss.
  }
  //fahert so lange nach unten wie es geht
  if (buttonPressed(red) == 0  && safty()) {
    delay(120);
    if (buttonPressed(red) == 0  && safty()) {
      startTime = millis();
      back(16000);
    }
  }
  //fahert von der mitte nach unten oder von unten in die mitte
  if (buttonPressed(blue) == 0  && safty()) {
    delay(120);
    if (buttonPressed(blue) == 0  && safty()) {
      startTime = millis();
      bonnie();
    }
  }

}
// Manuelle Steuerung. Nur P1 und P2 werden ueberwacht, magnetschloss offen
// Kann befehle Seriell lesen
void mLoop() {
  digitalWrite(Lock, LOW); // Magnetschkoss oeffnen
  while (true) {
    //gruen faehrt hoch
    if ((buttonPressed(green) == 0) && (signalRead(P1) == 0)) {
      while ((buttonPressed(green) == 0) && (signalRead(P1) == 0)) {
        goUp();
      }
      off();
    }
    //rot runter
    if ((buttonPressed(red) == 0) && (signalRead(P3) == 0)) {
      while ((buttonPressed(red) == 0) && (signalRead(P3) == 0)) {
        goDown();
      }
      off();
    }

    //blue und red resetten den Errorspeicher
    if (buttonPressed(blue) == 0) {
      delay(400);
      if (buttonPressed(blue) == 0) {
        overRun = 0;
        blinkGreen(5, 70);
        EEPROM.update(1, overRun);
      }
    }
  }
  {

    if (Serial.available() > 0) //"wenn ein Datenpaket geliefert wird"
    {
      data = Serial.read(); //liest die Daten

      if (data == '1') {
        goUp();
        delay(300);
        off();
      }
      if (data == '2') {
        goDown();
        delay(300);
        off();
      }
      if (data == '3') {
        printSensor();
      }
      Serial.flush(); //seriellen Puffer löschen
    }
  }
}

//Auslesung der Sensoren
void printSensor() {
  Serial.println("Signale");
  Serial.print("P1: ");
  Serial.println(signalRead(P1));
  Serial.print("P2A: ");
  Serial.println(signalRead(P2A));
  Serial.print("P2B: ");
  Serial.println(signalRead(P2B));
  Serial.print("P3: ");
  Serial.println(signalRead(P3));
  Serial.print("P4: ");
  Serial.println(signalRead(P4));
  Serial.println(" ");
  Serial.println("Taster");
  Serial.print("green: ");
  Serial.println(buttonPressed(green));
  Serial.print("red: ");
  Serial.println(buttonPressed(red));
  Serial.print("blue: ");
  Serial.println(buttonPressed(blue));
  Serial.println(" ");
  Serial.println("Klappen");
  Serial.print("S1: ");
  Serial.println(signalRead(S1));
  Serial.print("S2: ");
  Serial.println(signalRead(S2));
  Serial.println(" ");
  delay(800);
}
