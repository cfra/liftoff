Torben Schloß

V0.1

Hinzugefügt Aufbau und Funktionen
Noch nicht viel mir fehlt noch die Sicherheitslogik und ein paar Befehle muss ich noch suchen.

Die beiden Taster T1 und T2 starten die jeweiligen Befehle.
Die safty funktion stellt sicher, dass sich der Aufzug nicht bewegt und bricht den letzten Befehl ab.
Es gibt 2 Status LED's rot und gruen. Auf grün ist der Fahrstuhl betriebsbereit, auf rot ist eine der beiden Klappen geoeffnet.
Die Pins 2 und 3 muessen am Relais angeschlossen werden. Sie steuern den Motor.
P Sind die Signale an den Türmen, T sind die Taster, S für die Safty Klappen

Methoden

bring() faehrt den Aufzug nach oben
back() faehrt den Aufzug nach unten
bonnie() fahrt den Aufzug in die Mitte



V0.2 Beta

Der Motor sollte nun nach jedem Befehl bzw nachdem die Klappen geöffnet wurden, anhalten.
Der letzte Befehl muss dann erneut über die Knöpfe gestartet werden.
Man koennte nun den Befehl unterbrechen und ihn automatisch fortsetzen, indem man die Klappen schließt.

Die safty() methode überwacht nun die Klappen, schaltet die LED's und hält den Motor an.


V0.3
LOW und HIGH getauscht.
Kleinere Aenderungen

V0.4
Code improvements
Relais Sicherung eingebaut
T3 eingebaut
Comments erweitert