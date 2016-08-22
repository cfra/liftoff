Author T


Aufbau Funktionen und Pinbelegung

Der Aufzug wird von einem 0,47kW 230V Motor betrieben.
Die Ansteuerung läuft über einen 8fach 230V 10A Relais.
Die Relais werden ueber einen Arduino Mega 2560 geschaltet.
Zur Signal erfassung sind 3 Endschalter und 1 Reed-Kontakt verbaut.
Die Programmwahl erfolgt ueber 3 Taster. (gruen, rot und blau)
Zur Sicherheitsueberpruefung sind 2 Reed Kontakte zustaendig, welche Klappe 1 und Klappe 2 verbaut sind.
Desweiteren sind 2 Notaus schalter verbaut, diese werden zusaetzlich vom Arduino ueberprueft, um ein Programm zu stoppen.

Jede Funktion zur Steuerung des Motors besitzt einen maximalen runtime Wert (x),
sollte ein Signalgeber kaputt sein, stoppt der Aufzug
und kann nur durch einen Techniker wieder freigeschaltet werden.


An der Klappe in der Bar 
Am Aufzug befindet sich eine Lampe, welche leuchtet waerend der Auzuf faehrt



Signal			Pin				Bauteil				Funktion 				Erlaeuterung

down			2				Relais 1			Motor schalten			Aufzug runter
downSR			3				Relais 2			Motor schalten			Aufzug runter

up				4				Relais 3			Motor schalten			Aufzug hoch
upSR			5				Relais 4			Motor schalten			Aufzug hoch

L1				6				Relais 5			Motor Phase 1			Aufzug an
L2				7				Relais 6			Motor Phase 2			Aufzug an
	
Lock			8				Relais 7			Magnetschloss			Magnetschloss verriegelt Klappe 1
Lamp			9				Relais 8			Lampe					Lampe unter dem Aufzug geht an

LedR			10				rote	LED			leuchten				LED leuchtet oder blinkt
LedG			11				gruene 	LED			leuchten				LED leuchtet oder blinkt

P1				24				Endschalter			oberes 		Endsignal	Signal am Turm 1 oben
P2A				25				Rollenendschalter	mittleres 	Signal		Signal am Turm 1 mitte
P2B				26				Rollenendschalter	-			-										
P3				27				Endschalter			unteres 	Signal		Signal am Turm 1 unten
P4				28				Reed-Kontakt		Turm 2 		Signal		Signal am Turm 2 oben

T1				30				Taster gruen		bring(x)				Taster Keller
T2				31				Taster rot			back(x)					Taster Keller
T3				32				Taster blau			my(x),bonnie(x)			Taster Bar

S1				36				Reed-Kontakt		Turm 1 schliessen		Klappe Bar
S2				37				Reed-Kontakt		Turm 1 schliessen		Klappe Keller

N1				40				Notaus Relais		Notausueberwachung		Ueberwacht ob der Notaus betätigt wurde


Funktionen		Ausloesung				Aktion						

goDown()		T2, T3					Relais schalten, Lampe ein	down,downSR,L1,L2,P3,Lamp
goUp()			T1, T3					Relais schalten, Lampe ein	up,upSR,L1,L2,P1, Lamp
off()			P1,P2A,P3, P4			Relais schalten, Lampe aus	up,upSR,down,downSR,L1,L2,Lamp

bring(x)		Gestartet durch T1		der Aufzug faehrt hoch 		P1, P4
back(x)			Gestartet durch T2,T3 	der Aufzug faehrt runter	P3
my(x)			Gestartet durch T3 		der Aufzug faehrt hoch 		P2A
bonnie(x)		Gestartet durch T3	 	der Aufzug faehrt runter 	P3

safty()			main loop				ueberwachung der Klappen	S1,S2
isMid()			my(x), bonnie(x)		Magnetschloss schalten		P2A
maxRuntime(x)	T1,T2,T3				Laufzeit berechnung			Int x, P1,P2A,P3,
emergencyStop()	main loop				Notausueberwachung			N1
			


Die safty funktion stellt sicher, dass sich der Aufzug nicht bewegt und bricht den letzten Befehl ab.
Es gibt 2 Status LED's rot und gruen. Auf grün ist der Fahrstuhl betriebsbereit, auf rot ist eine der beiden Klappen geoeffnet.



goDown()					
goUp()
off()	

goDown und goDown schalten die Richtung des Motors, sollten P1 oder P3 nicht betaetigt sein.
Setzt sich der Aufzug in bewegung, wird zusaetzlich die Lampe unter dem Aufzug angeschaltet.

off schaltet alle Relais fuer die Motorensteuerung und die Lampe aus, und resettet die runtime.

bring(x)

bring wird durch den gruenen Taster im Keller gestartet und
faehrt den Aufzug nach oben, dabei entlaed er sich an P1.

Nach 3 sekunden startet die Funktion back().

bring laueft solange
bis
		P1 					== false
		P4 					== true
		safty() 			== true
		maxRuntime()		== true
		emergencyStop() 	== false
		


back(x)			
 Wird durch den roten Schalter im Keller gestartet und den blauen an der Bar.
	Ist bestandteil der Funktionen bonnie() und bring().
	
	laueft solange bis P3 == false

my(x)

wird durch die Funktion bonnie(x) gestartet

bonnie(x)		
wird durch den blauen Schalter an der Bar gestartet
ist isMid() == true fahert der Aufzug in den Keller und verriegelt das Magnetschloss
ist isMid() == false fahert der Aufzug vom Keller zur Bar und entriegelt das Magnetschloss

safty()
laueft immer in der Mainloop und in jeder der 4 Funktion zur Ansteuerung,
sollte eine Klappe geoeffent werden, stoppt das Programm sofort und der Aufzug wird angehalten.

isMid()
der Schalter P2A im Turm auf hoehe der Bar ueberprueft ob sich der Aufzug in der Mitte befindet

maxRuntime(x)
jede Funktion zur Bewegungssteuerung hat einen maximalen Laufzeitwer (maxRuntime) wird dieser ueberschritten,
stoppt der Aufzug und kann nur im debug modus resettet werden.

emergencyStop()
wird der Notausknopf betaetigt, stoppt jede laufendene Funktion (bring back my bonnie)




