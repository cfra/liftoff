//Aktiviert durch green, bringt der Aufzug eine Kiste BIER nach oben, nahc der entladung (3 sekunden spaeter) begibt der Aufzug sich in den Keller
void bring(int x) {
  if (digitalRead(P4) == 0) {
    blinkGreen(3, 200); //// <><><><><><><><><><><><><><><><><><><><><><><><><><><><> bedienungsanleitung!
  }
  else {
    while (digitalRead(P1) == 0 && digitalRead(P4) == 1 &&
           safty() && maxRunTime(x)) {
      currentTime = millis() - startTime;
      goUp();
    }
    off();
    delay(3000); // Kiste rutscht vom Aufzug -- hoffentlich!
    startTime = millis(); //time reset
    back(16000);
  }
}
//Aktiviert durch red faehrt den Aufzug nach unten zu P3
void back(int x) {
  while (digitalRead(P3) == 0 && safty() && maxRunTime(x) ) {
    currentTime = millis() - startTime;
    goDown();
  }
  off();
}

//Wenn der Aufzug in der Mitte ist, faehrt er in den Keller,
// wenn er im Keller ist, faehrt er in die Mitte
void bonnie() {
  if (isMid() && safty() == true) {
    digitalWrite(M, HIGH); // Magnetschkoss verriegeln
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
    digitalWrite(M, LOW); // Magnetschkoss oeffnen
    }
  }
}
