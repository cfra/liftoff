/*
  
 LED steuerung und Fehlercodes

Beschreibung                        Code                              Behebung
  
Bereit                        gruene LED      leuchtet                /
Klappe geoeffnet              rote LED        blinkt langsan          Klappe schliessen
Kiste enladen                 gruene LED      blinkt langsam          Kiste entladen
Zeitueberschreitung           rote LED        blinkt schnell          Techniker rufen

*/







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
