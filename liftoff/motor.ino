//Motorenbefehle
//goUp und goDown
void goUp() {
  if (digitalRead(P1)  != 1) { // Hier ist mir die entprellte variante gerade zu riskant falls was damit nicht geht :3
    digitalWrite(L1,      LOW);
    digitalWrite(L2,      LOW);
    digitalWrite(up,      LOW);
    digitalWrite(upSR,    LOW);
    digitalWrite(Lamp,    LOW); //Lampe ein
  }
  else {
    off();
  }
}
// L1 L2 aendern
void goDown() {  
  if (digitalRead(P3) != 1) { // Hier ist mir die entprellte variante gerade zu riskant falls was damit nicht geht :3
    digitalWrite(L1,      LOW);
    digitalWrite(L2,      LOW);
    digitalWrite(down,    LOW);
    digitalWrite(downSR,  LOW);
    digitalWrite(Lamp,    LOW); //Lampe ein
  }
  else {
    off();
  }
}



//off schaltet beide Relais aus.
void off() {
  
  digitalWrite(L1,      HIGH);
  digitalWrite(L2,      HIGH);
  digitalWrite(up,      HIGH);
  digitalWrite(upSR,    HIGH);
  digitalWrite(down,    HIGH);
  digitalWrite(downSR,  HIGH);
  digitalWrite(Lamp,    HIGH); //Lampe aus
  Serial.println(millis() - startTime);
  currentTime = 0; //reset der laufzeit
  delay(251); //die Relais duerfen den Motor darf nicht zu schnell schalten (250ms)
}
