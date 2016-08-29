// Manuelle Steuerung. Nur P1 und P2 werden ueberwacht, magnetschloss offen
// Kann befehle Seriell lesen
void mLoop() {
  digitalWrite(Lock, LOW); // Magnetschkoss oeffnen
  while (true) {


    data = Serial.read(); //liest die Daten

    if (data == '1') {
      Serial.println("bring");

      bring(16000);

      Serial.println("done");
    }
    if (data == '2') {
      Serial.println("back");

      back(16000);
      Serial.println("done");
    }
    if (data == '3') {
      printSensor();
    }
    Serial.flush(); //seriellen Puffer löschen

    //gruen faehrt hoch
    if ((green.read() == 0) && (p1.read() == 0)) {
      while ((green.read() == 0) && (p1.read() == 0)) {
        goUp();
      }
      off();
    }
    //rot runter
    if ((red.read() == 0) && (p3.read() == 0)) {
      while ((red.read() == 0) && (p3.read() == 0)) {
        goDown();
      }
      off();
    }

    //blue und red resetten den Errorspeicher
    if (blue.read() == 0) {
      delay(400);
      if (blue.read() == 0) {
        overRun = 0;
        blinkGreen(5, 70);
        EEPROM.update(0, overRun);
      }
    }

    digitalWrite(ledG, HIGH);
    digitalWrite(ledR, HIGH);

  }
}
