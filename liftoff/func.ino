//Ueberwachungsfunktionen
//isMid ueberprueft ob der Aufzug sich in der mittleren Posititon befindet, und schaltet ggf. das Magnetschloss
boolean isMid() {
  if (digitalRead(P2A) == 0) {
    digitalWrite(M ,  HIGH);
    return false;
  } else if (digitalRead(P2A) == 1) {
    digitalWrite(M,   LOW);
    return true;
  }
}

//Sobald eine der Beiden Klappen geoeffnet wird, Stoppt der Aufzug
//Sind die Klappen wieder geschlossen, kann durch erneutes druecken der Knoepfe der Aufzug wieder gestartet werden.  <><><><><><><><><><><><><><><><><><><><><><><><><><><><> Bedienungsanleitung

boolean safty() {

  if ((signalRead(S1) == 0) && (signalRead(S2) == 0)) {
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    return true;
  }
  else {
    off();
    blinkRed(1, 200);
    return false;
  }

}

//Die 4 Programme der Knoepfe liefern einen maximalen maxRunTime Wert (x), wird dieser überschritten, schaltet das system aus.
//Das Errorflag wird auf EEPROMM (1,1) gespeichert und muss quittiert werden.
//x darf nicht kleiner 0 sein
boolean maxRunTime(int x) {
  if (currentTime > x && EEPROM.read(1) == 0) {
    off();
    overRun = 1;
    //EEPROM.write(1, overRun);
    blinkRed(200, 60); // <><><><><><<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> bedienungsanleitung!
    return false;
  }
  else return true;
}


//led blink x Wiederholung, y milliseconds
void blinkGreen(int x, int y) {
  int c = 0;
  while (c < x) {
    digitalWrite(ledG, HIGH);
    delay(y);
    digitalWrite(ledG, LOW);
    delay(y);
    c++;
  }
}

void blinkRed(int x, int y) {
  int c = 0;
  while (c < x) {
    digitalWrite(ledR, HIGH);
    delay(y);
    digitalWrite(ledR, LOW);
    delay(y);
    c++;
  }
}