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
        EEPROM.update(0, overRun);
      }
    }

    digitalWrite(ledG, HIGH);
    digitalWrite(ledR, HIGH);

  }


}
