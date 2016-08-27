//Aktiviert durch green, bringt der Aufzug eine Kiste BIER nach oben, nahc der entladung (3 sekunden spaeter) begibt der Aufzug sich in den Keller
void bring(int x) {
  if (P4state == 0) {
    blinkGreen(3, 200); ////
  }
  else {
    currentTime = millis() - startTime;
    while (P1state == 0 && P4state == 1 &&
           safty() && maxRunTime(x)) {
      bufferInputs();
      goUp();
    }
    P4activated = 0;
  }
  off();
}

//Aktiviert durch red faehrt den Aufzug nach unten zu P3
void back(int x) {
  currentTime = millis() - startTime;
  while (digitalRead(P3) == 0 && safty() && maxRunTime(x) ) {
    bufferInputs();
    if (P4state == 0) {
      P4activated = 1;
    }
    goDown();
  }
  off();
}

//Wenn der Aufzug in der Mitte ist, faehrt er in den Keller,
// wenn er im Keller ist, faehrt er in die Mitte
void bonnie() {
  if (isMid() && safty()) {
    digitalWrite(Lock, HIGH); // Magnetschkoss verriegeln
    back(8000);
  }
  else if (isMid() == false && digitalRead(P3) == 1 && safty()) {
    my(8000);
  }
  //Falls er ueber P2 steht
  else {
    back(16000);
  }
  off();
}

//Funktion von bonnie
void my(int x) {
  while (isMid() == false && signalRead(P1) == 0 &&
         safty() && maxRunTime(x)) {
    currentTime = millis() - startTime;
    goUp();
    if (isMid()) {
      digitalWrite(Lock, LOW); // Magnetschkoss oeffnen
    }
  }
}

//Tests

void testBonnie() {
  isMid();
  bring(9000);
  delay(3000);
  while (isMid() == false && digitalRead(P3) == 0 &&
         safty()) {
    goDown();
  }
  off();
  isMid();
}


