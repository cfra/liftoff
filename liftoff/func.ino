//Ueberwachungsfunktionen
//isMid ueberprueft ob der Aufzug sich in der mittleren Posititon befindet, und schaltet ggf. das Magnetschloss
boolean isMid() {
  if (digitalRead(P2A) == 0) {
    digitalWrite(Lock ,  HIGH);
    return false;
  } else if (digitalRead(P2A) == 1) {
    digitalWrite(Lock,   LOW);
    return true;
  }
}

//Sobald eine der Beiden Klappen geoeffnet wird, Stoppt der Aufzug
//Sind die Klappen wieder geschlossen, kann durch erneutes druecken der Knoepfe der Aufzug wieder gestartet werden.  <><><><><><><><><><><><><><><><><><><><><><><><><><><><> Bedienungsanleitung

boolean safty() {

  if ((signalRead(S1) == 0)) {  //  && (signalRead(S2) == 0)
    digitalWrite(ledG, HIGH);
    digitalWrite(ledR, LOW);
    return true;
  }
  else {
    digitalWrite(ledG, LOW);
    off();
    blinkRed(1, 200);
    return false;
  }

}
boolean emergencyStop() {
  if (digitalRead(N1) == 1) {
    return false;
  }
  else return true;
}


//Die 4 Programme der Knoepfe liefern einen maximalen maxRunTime Wert (x), wird dieser überschritten, schaltet das system ab.
//Das Errorflag wird auf EEPROMM (1,1) gespeichert und muss quittiert werden.
//maxRunTime Wert (x) darf nicht kleiner 0 sein
boolean maxRunTime(int x) {
  //  if ((millis()- startTime) > x && EEPROM.read(1) == 0) {
  //    off();
  //    overRun = 1;
  //    //EEPROM.write(1, overRun);
  //    blinkRed(200, 60); // <><><><><><<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> bedienungsanleitung!
  //    return false;
  //  }
  //  else return true;
  return true;
}

void timeFunction(int x) {

  int timer1 = 0;
  int timer2 = 0;

}

//#define P4_LOW_COUNT 1024
//void updateP4activated(){
//
//static unsigned int p4_low = 0
//
//if (digitalRead(P4) == 0) {
//    if (p4_low < P4_LOW_COUNT)
//        p4_low++;
//} else {
//    p4_low = 0;
//}
//if (p4_low == P4_LOW_COUNT)
//    P4activated = 1;


#define BUFFERED_PIN(PIN,SAMPLE_COUNT) \
  byte PIN##state = -1; \
  void buffer##PIN() { \
    static unsigned int state_dur = 0; \
    static byte last_state = -1; \
    byte current_state; \
    \
    current_state = digitalRead(PIN); \
    if (current_state != last_state) { \
      last_state = current_state; \
      state_dur = 0; \
    } else { \
      if (state_dur < SAMPLE_COUNT) { \
        state_dur++; \
      } \
      if (state_dur == SAMPLE_COUNT) { \
        PIN##state = current_state; \
      } \
    } \
  }

BUFFERED_PIN(P1, 2048);
BUFFERED_PIN(P4, 2048);

void bufferInputs() {
  bufferP1();
  bufferP4();
}
