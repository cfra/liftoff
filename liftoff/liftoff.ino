#include <Debounce.h>
#include <PinDebouncer.h>

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

#define ledR        10
#define ledG        11

#define P1          24
#define P2A         25
#define P2B         26 // not in use
#define P3          27
#define P4          28

#define T1          30
#define T2          31
#define T3          32


#define S1          36
#define S2          37

#define N1          40

PinDebouncer p1(P1,10);
PinDebouncer p2A(P2A, 10);
PinDebouncer p2B(P2B, 10);
PinDebouncer p3(P3, 10);
PinDebouncer p4(P4,10);

PinDebouncer green(T1, 10);
PinDebouncer red(T2, 10);
PinDebouncer blue(T3, 10);

PinDebouncer s1(S1, 10);
PinDebouncer s2(S2, 10);

PinDebouncer n1(N1, 40);

char data = ' '; //Serieller char

//Timervariablen
int startTime =    0;
int currentTime =  0;
int saftyStauts =  0;
int counter = EEPROM.read(0);
byte overRun = EEPROM.read(1); //Error durch Zeitausloesung

byte p4activated = 1; //Nehme bei boot an dass keine Kiste im Turm 2 ist
void p4event(enum DebounceEdgeType edge)
{
  if (edge == FallingEdge) {
    p4activated = 1;
  }
}

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

  pinMode(T1,       INPUT); //Taster gruen
  pinMode(T2,       INPUT); //Taster rot
  pinMode(T3,       INPUT); //Taster blau

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
  //digitalWrite(Lock,      HIGH);
  digitalWrite(Lamp,      HIGH);

  //umschaltung von L1 & L2 auf LOW ( spannung fällt ab)
  digitalWrite(L1,        HIGH);
  digitalWrite(L2,        HIGH);

  //interner pullUp resitor fuer alle read pins
  digitalWrite(P1,        HIGH);
  digitalWrite(P2A,       HIGH);
  digitalWrite(P2B,       HIGH);
  digitalWrite(P3,        HIGH);
  digitalWrite(P4,        HIGH);

  digitalWrite(T1,        HIGH);
  digitalWrite(T2,        HIGH);
  digitalWrite(T3,        HIGH);

  digitalWrite(S1,        HIGH);
  digitalWrite(S2,        HIGH);
  digitalWrite(N1,        HIGH);

  digitalWrite(ledR,      LOW);
  digitalWrite(ledG,      LOW);

  // Only start debouncer after inputs have been configured
  Debounce::start();
  // Wait for debouncer to settle
  delay(50);
  // Register p4activated handler
  p4.setHandler(p4event);

  //Start sequenz
  printSensor();
  blinkGreen(5, 300);
  //  mLoop();

  //Nachdem das System gestartet wurde, blinkt die rote LED 5 mal.
  //Wird green und red gedrueckt, bevor die gruene LED blinkt, startet der Manuelle modus "mLoop()"
  //er kann nur durch neustart verlassen werden
  if (green.read() == 0 && red.read() == 0) {
    digitalWrite(ledR,    HIGH);
    digitalWrite(ledG,    HIGH);
    delay(1200);
    digitalWrite(ledR,    LOW);

  }
  blinkRed(5, 200);
}

//Main
void loop() {
  //Die klappen werden immer ueberwacht
  safty();

  //fahert von unten nach oben entlaed und fahert wieder runter
  if (green.read() == 0 && safty()) {
    delay(120); // Knopf muss xms gedrückt sein
    if (green.read() == 0 && p4.read() == 1 && p4activated == 1 && safty()) {
      startTime = millis(); //Laufzeit berechnung
      bring(16000);
      delay(3000); // Kiste rutscht vom Aufzug -- hoffentlich!
      startTime = millis(); //time reset
      back(16000);
    } else if (p4.read() != 1 || p4activated != 1) {
      blinkGreen(4 , 150); //zeigt an, dass eine Kiste oben entladen werden muss.
    }
  }

  //fahert so lange nach unten wie es geht
  if (red.read() == 0  && safty()) {
    delay(120);
    if (red.read() == 0  && safty()) {
      startTime = millis();
      back(16000);
    }
  }
  //  //fahert von der mitte nach unten oder von unten in die mitte
  //  if (blue.read() == 0  && safty()) {
  //    delay(120);
  //    if (blue.read() == 0  && safty()) {
  //      startTime = millis();
  //      bonnie();
  //    }
  //  }
  //Test
  //if (blue.read() == 0  && safty()) {
  //    delay(120);
  //    if (blue.read() == 0  && safty()) {
  //      startTime = millis();
  //      testBonnie();
  //    }
  //  }
}


//Auslesung der Sensoren
void printSensor() {
  Serial.println("Signale");
  Serial.print("P1: ");
  Serial.println(digitalRead(P1));
  Serial.println(p1.read());
  Serial.print("P2A: ");
  Serial.println(digitalRead(P2A));
  Serial.println(p2A.read());
  Serial.print("P2B: ");
  Serial.println(digitalRead(P2B));
  Serial.println(p2B.read());
  Serial.print("P3: ");
  Serial.println(digitalRead(P3));
  Serial.println(p3.read());
  Serial.print("P4: ");
  Serial.println(digitalRead(P4));
  Serial.println(p4.read());
  Serial.println(" ");
  Serial.println("Taster");
  Serial.print("green: ");
  Serial.println(digitalRead(T1));
  Serial.println(green.read());
  Serial.print("red: ");
  Serial.println(digitalRead(T2));
  Serial.println(red.read());
  Serial.print("blue: ");
  Serial.println(digitalRead(T3));
  Serial.println(blue.read());
  Serial.println(" ");
  Serial.println("Klappen");
  Serial.print("S1: ");
  Serial.println(digitalRead(S1));
  Serial.println(s1.read());
  Serial.print("S2: ");
  Serial.println(digitalRead(S2));
  Serial.println(s2.read());
  Serial.println(" ");
  Serial.print("Fehlerspeicher: ");
  Serial.println(EEPROM.read(1));
  delay(800);
}
