#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <chrono>  
#include <thread>
#include<memory>
#include "Spielfeld.cpp"

class Spiel {
private:
	std::vector<std::shared_ptr<Spieler>> spielerVector;
	std::unique_ptr<Spielfeld> spielfeld;

public:
	Spiel() : spielfeld(nullptr) {
	}

	Spiel(std::unique_ptr<Spielfeld> sf) : spielfeld(std::move(sf)) {}
	

	void hinzufügenSpieler(const std::shared_ptr<Spieler>& spieler) { spielerVector.push_back(spieler);	}
	std::shared_ptr<Spieler> getSpieler(int x) const { return spielerVector[x]; }

	Spielfeld& getSpielfeld() { return *spielfeld; }
	void setSpielfeld(std::unique_ptr<Spielfeld> sf) { spielfeld = std::move(sf); }

	bool stringToBool(const std::string& s) const {
		return (s == "true");
	}


	std::vector<std::shared_ptr<Spieler>> getSpielerVector(){
		return spielerVector;
	};

	const std::string enumToString(const Farbe& farbe) const {
		switch (farbe) {
		case Farbe::Rot: return "Rot";
		case Farbe::Gruen: return "Gruen";
		case Farbe::Blau: return "Blau";
		case Farbe::Gelb: return "Gelb";
		case Farbe::Magenta: return "Magenta";
		case Farbe::Cyan: return "Cyan";
		case Farbe::Weiss: return "Weiss";
		case Farbe::Reset: return "Reset";
		default: return "Unbekannt";
		}
	}

	const Farbe stringToEnum(const std::string& s) const {
		if (s == "Rot") { return Farbe::Rot; }
		else if (s == "Gruen") { return Farbe::Gruen; }
		else if (s == "Blau") { return Farbe::Blau; }
		else if (s == "Gelb") { return Farbe::Gelb; }
		else if (s == "Magenta") { return Farbe::Magenta; }
		else if (s == "Cyan") { return Farbe::Cyan; }
		else if (s == "Weiss") { return Farbe::Weiss; }
		else if (s == "Reset") { return Farbe::Reset; }
		else { return Farbe::Weiss; }
	}

	void spielSpeichern() {
		std::ofstream spielstand("spielstand.txt");
		if (spielstand.is_open()) {
			for (const auto& spieler : spielerVector) {
				spielstand << "Spieler: " << spieler->getId() << "," << spieler->getName() << "," << spieler->getIsAI() << ",";
				spielstand << enumToString(spieler->getFarbe()) << "," << spieler->getScore() << std::endl; //hier Frage ob Score speichern oder sp�ter einfach berechnen aus den feldern? + Frage wegen Enum Farbe obs so passt
			}
			spielstand << "Feldgroesse: " << getSpielfeld().getSize() << std::endl;
			for (int i = 0; i < getSpielfeld().getSize(); i++) {
				for (int j = 0; j < getSpielfeld().getSize(); j++) {
					if (getSpielfeld().getFeld(i, j).getOwner() == nullptr) {
						spielstand << "Feld: " << i << "," << j << "," << getSpielfeld().getFeld(i, j).getAnzahl() << ",";
						spielstand << -1 << std::endl;
					}
					else {
						spielstand << "Feld: " << i << "," << j << "," << getSpielfeld().getFeld(i, j).getAnzahl() << ",";
						spielstand << getSpielfeld().getFeld(i, j).getOwner()->getId() << std::endl;
					}

				}
			}
			spielstand.close();
			std::cout << "Daten wurden in 'spielstand.txt' gespeichert!" << std::endl;
		}
		else {
			std::cerr << "Fehler beim oeffnen der Datei!" << std::endl;
		}
	}

	void spielLaden() {
		std::ifstream fReader("spielstand.txt");
		std::string line;
		if (!fReader.is_open()) {
			std::cout << "Datei konnte nicht geoeffnet werden!" << std::endl;
			return;
		}
		if (fReader.is_open()) {
			while (std::getline(fReader, line)) {
				std::istringstream linestream(line);
				std::string entry;
				int spielerId;
				std::string name;
				bool isAI;
				Farbe farbe;
				int score;
				if (line.find("Spieler: ") == 0) {
					std::istringstream specStream(line.substr(8));

					if (std::getline(specStream, entry, ',')) {
						spielerId = std::stoi(entry);
						std::cout << "gelesene Player ID: " << spielerId << std::endl;
					}
					if (std::getline(specStream, entry, ',')) {
						name = entry;
						std::cout << "gelesener Player name: " << name << std::endl;
					}
					if (std::getline(specStream, entry, ',')) {
						isAI = stringToBool(entry);
						std::cout << "gelesene Player AI: " << isAI << std::endl;
					}
					if (std::getline(specStream, entry, ',')) {
						farbe = stringToEnum(entry);
						std::cout << "gelesene Player Farbe: " << entry << std::endl;
					}
					if (std::getline(specStream, entry, ',')) {
						score = std::stoi(entry);
						std::cout << "gelesener Player score: " << score << std::endl;
					}
					hinzufügenSpieler(std::make_shared<Spieler>(farbe, name, isAI, spielerId));
				}
				else if (line.find("Feldgroesse: ") == 0) {
					std::istringstream specStream(line.substr(12));
					int zeilen;
					if (std::getline(specStream, entry, ',')) {
						zeilen = std::stoi(entry);
						std::cout << "gelesene SpielfeldZeilen: " << zeilen << std::endl;
					}
					setSpielfeld(std::make_unique<Spielfeld>(zeilen));
				}
				else if (line.find("Feld: ") == 0) {
					std::istringstream specStream(line.substr(6));
					int anzahlPunkte;
					int ownerId;
					int zeile;
					int spalte;
					if (std::getline(specStream, entry, ',')) {
						zeile = std::stoi(entry);
						std::cout << "gelesene Feld Zeile: " << zeile << std::endl;
					}
					if (std::getline(specStream, entry, ',')) {
						spalte = std::stoi(entry);
						std::cout << "gelesene Feld Spalte: " << spalte << std::endl;
					}
					if (std::getline(specStream, entry, ',')) {
						anzahlPunkte = std::stoi(entry);
						std::cout << "gelesene Feld Punkte: " << anzahlPunkte << std::endl;
					}
					if (std::getline(specStream, entry, ',')) {
						ownerId = std::stoi(entry);
						if (ownerId == -1) {
							getSpielfeld().getFeld(zeile, spalte).setAnzahl(anzahlPunkte);
							getSpielfeld().getFeld(zeile, spalte).setOwner(nullptr);
						}
						else {
							for (auto& possibleOwner : spielerVector) {
								if (possibleOwner->getId() == ownerId) {
									getSpielfeld().getFeld(zeile, spalte).setAnzahl(anzahlPunkte);
									getSpielfeld().getFeld(zeile, spalte).setOwner(possibleOwner);
									std::cout << "eingetragene Owner id: " << getSpielfeld().getFeld(zeile, spalte).getOwner()->getId() << std::endl;
								}
							}
						}
						std::cout << "gelesene Feld Owner ID: " << ownerId << std::endl;
					}
				}
			}
		}
		fReader.close();
		std::cout << "Spielstand erfolgreich geladen!" << std::endl;
		return;
	}

	void spielInitialisieren() {
		
		bool gültigeAnzahlSpielerEingabe = false;
		int anzahlSpieler;

		// Eingabe der Spieleranzahl mit Überprüfung, ob die Zahl zwischen 1 und 4 liegt
		while (!gültigeAnzahlSpielerEingabe) {
			std::cout << "Wie viele Spieler machen mit? (1-4)" << std::endl;
			std::cin >> anzahlSpieler;

			// Eingabe validieren
			if (std::cin.fail() || anzahlSpieler < 1 || anzahlSpieler > 4) {
				std::cin.clear(); // Fehlerstatus zurücksetzen
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Puffer leeren
				std::cout << "Ungueltige Eingabe! Bitte gib eine Zahl zwischen 1 und 4 ein." << std::endl;
			}
			else {
				gültigeAnzahlSpielerEingabe = true; // Gültige Eingabe
			}
		}
		
		if(anzahlSpieler <= 2){
			setSpielfeld(std::make_unique<Spielfeld>(5));
		}
		else {
			setSpielfeld(std::make_unique<Spielfeld>(7));
		}
		for (int i = 0; i < anzahlSpieler; i++) {
			std::string spielerName;
			std::string spielerFarbe;
			std::string teil;
			std::cout << "--------------------------------------" << std::endl;
			std::cout << "Bitte geben Sie einen Namen ein:" << std::endl;
			while (std::cin >> teil) {
				spielerName += teil;
				if (std::cin.peek() == '\n') {
					break; // Ende der Zeile prüfen
				}
				else {
					spielerName += " ";
				}
			}

			// Rot als Farbe für normale Spieler ausgeschlossen, da KI = Rot
			
			std::cout << "Bitte geben Sie eine Farbe an (Gruen|Blau|Gelb|Magenta|Cyan):" << std::endl;
			std::cin >> spielerFarbe;
			while (spielerFarbe == "Rot" || stringToEnum(spielerFarbe) == Farbe::Weiss || stringToEnum(spielerFarbe) == Farbe::Reset
			 || !istFarbeVerfuegbar(stringToEnum(spielerFarbe))) {

				if (spielerFarbe == "Rot") {
					std::cout << "Bitte geben Sie eine Farbe ausser 'Rot' an:" << std::endl;
				}
				if (istFarbeVerfuegbar(stringToEnum(spielerFarbe)) == false) {
					std::cout << "Farbe bereits vergeben, bitte waehlen Sie eine andere Farbe" << std::endl;
				}
				else {
					std::cout << "Bitte geben Sie eine gueltige Farbe an:" << std::endl;
				}
				std::cin >> spielerFarbe;
			}
			hinzufügenSpieler(std::make_shared<Spieler>(stringToEnum(spielerFarbe), spielerName, false));
		}

		char KI;
		bool gültigeKIEingabe = false;

		if (anzahlSpieler == 1) {
			// Wenn nur 1 Spieler, wird der KI-Spieler automatisch hinzugefügt
			std::cout << "Da nur 1 Spieler mitmacht, wird automatisch ein KI-Spieler hinzugefügt." << std::endl;
			hinzufügenSpieler(std::make_shared<Spieler>(stringToEnum("Rot"), "Computer", true));
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		}
		else {
			// Bei 2 bis 4 Spielern wird gefragt, ob ein KI-Spieler hinzugefügt werden soll
			while (!gültigeKIEingabe) {
				std::cout << "KI Spieler hinzufuegen? Y - Ja  N - Nein:" << std::endl;
				std::cin >> KI;

				KI = std::toupper(KI);

				if (KI == 'Y' || KI == 'N') {
					gültigeKIEingabe = true;
					if (KI == 'Y') {
						// KI Spieler erstellen mit IsAI = true und Name "Computer"
						hinzufügenSpieler(std::make_shared<Spieler>(stringToEnum("Rot"), "Computer", true));
					}
					else {
						std::cout << "Es wurde keine KI erstellt" << std::endl;
					}
				}
				else {
					std::cout << "Ungueltige Eingabe, bitte 'Y' fuer Ja oder 'N' fuer Nein eingeben." << std::endl;
				}
			}
		}

	}

	static int letterToNumber(char letter) {
		switch (std::toupper(letter)) {
		case 'A': return 0;
		case 'B': return 1;
		case 'C': return 2;
		case 'D': return 3;
		case 'E': return 4;
		case 'F': return 5;
		case 'G': return 6;
		case 'H': return 7;
		case 'I': return 8;
		case 'J': return 9;
		default: return -1; // Ungültiger Buchstabe
		}
	}

	bool isValidNumber(char number) {
    int num = number - '0';  // Konvertiere char zu int
    int size = getSpielfeld().getSize();

    if (size == 5) {
			return num >= 1 && num <= 5;  // Für 5x5 Spielfeld, Zahlen von 0 bis 4
		} else if (size == 7) {
			return num >= 1 && num <= 7;  // Für 10x10 Spielfeld, Zahlen von 0 bis 9
		}
    return false;  // Wenn die Größe unerwartet ist
	}

	bool isValidLetter(char letter) {
    letter = std::toupper(letter);  // Konvertiere Buchstaben in Großbuchstaben
    int size = getSpielfeld().getSize();

	if (size == 5) {
			return letter >= 'A' && letter <= 'E';
		} else if (size == 7) {
			return letter >= 'A' && letter <= 'G';
		}
    return false;  // Wenn die Größe unerwartet ist
	}

	std::array<int, 2> getInput() {
		while (true) {
			std::string input;
			std::cin >> input;
			if (input.length() == 2 && isValidLetter(input[0]) && isValidNumber(input[1])) {
				std::array<int, 2> koordinaten;
				koordinaten[0] = static_cast<int>(input[1] - 49); //Weil ASCII bei 48 anfängt und Erste Feld = A1
				koordinaten[1] = letterToNumber(input[0]);
				return koordinaten;
			}
			else {
				std::cout << "Eingabe muss validen Buchstaben und Zahlen innerhalb der Spielgrenzen haben!" << std::endl;
			}
		}
	}
	
	void spielen() {
		getSpielfeld().printSpielfeld();
		for (auto& spieler : spielerVector) {
			ersterZug(spieler);
		}
		while (!finished()) {
			for (auto& spieler : spielerVector) {
				if (!finished()) {
                zug(spieler);
				} else {
					std::cout << "Spiel zu Ende" << std::endl;
					return;
            		}
			}
		}
	}
	// Zugmethoden für Spieler und KI

	void zug(std::shared_ptr<Spieler>& spieler) {
	if(besitztSpielerFelder(spieler)){
		if (!spieler->getIsAI()) { // Zug für Nicht-KI Spieler, wenn KI-Spieler, dann KIZug(spieler), also else-Anweisung
		std::cout << spieler->getName() << ", bitte waehle ein Feld" << std::endl; //abfrage ob Spiel beenden und speichern
		std::array<int, 2> koordinaten = getInput();
			if (getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).getOwner() == spieler) {
				getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).hinzufuegen();
				getSpielfeld().splash();
			}
			else {
				std::cout << "Waehle ein Feld, welches dir gehoert!" << std::endl;
				zug(spieler);
			}
		} 
		else {
			KIZug(spieler);
		}
	}
	else {
		std::cout << spieler->getName() << " besitzt keine Felder mehr." << std::endl;
	}
	}

	void KIZug(std::shared_ptr<Spieler>& spieler) {
		if(besitztSpielerFelder(spieler)){
        std::vector<Feld*> felderDesSpielers = besetzteFelder(spieler);
			
			std::cout << spieler->getName() << " waehlt ein Feld..." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1400));
			
            // Erhöhe das erste Feld, das der Spieler besitzt
            Feld* feld = felderDesSpielers[0];
            feld->hinzufuegen();
			spielfeld->splash();
			
		}
		else {
		std::cout << spieler->getName() << " besitzt keine Felder mehr." << std::endl;
		}
    }

	// Erste Zugmethoden für Spieler und KI 

	void ersterZug(std::shared_ptr<Spieler>& spieler) {
		if (!spieler->getIsAI()) { // Erster Zug Methode für nicht KI Spieler, wenn KI ,dann else Anweisung
		std::cout << spieler->getName() << ", bitte waehle ein Startfeld" << std::endl;
		std::array<int, 2> koordinaten = getInput();
		if (getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).getAnzahl() == 0) {
			getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).setOwner(spieler);
			getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).setAnzahl(3);
			getSpielfeld().splash();
		}
		else {
			std::cout << "Waehle ein Feld, dass niemandem gehoert!" << std::endl;
			ersterZug(spieler);
		}

	}	else {
			ersterKIZug(spieler);
		}
	}

	void ersterKIZug(std::shared_ptr<Spieler>& spieler) {
            // Wenn KI kein Feld hat, alle FREIEN Felder in freieFelder speichern
            std::vector<Feld*> freieFelder;
			std::cout << spieler->getName() << " waehlt ein Startfeld..." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1400));

            for (int i = 0; i < spielfeld->getSize(); i++) {
                for (int j = 0; j < spielfeld->getSize(); j++) {
                    Feld& feld = spielfeld->getFeld(i, j);
                    if (feld.getOwner() == nullptr) {
                        freieFelder.push_back(&feld);
                    }
                }
            }
			// Aus freien Feldern per Zufall eins auswählen für den Anfang
            if (!freieFelder.empty()) {
                std::random_device rd;  // Zufallszahlengenerator initialisieren
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, freieFelder.size() - 1);
                int index = dis(gen);
                Feld* feld = freieFelder[index];
                feld->setOwner(spieler);
                feld->setAnzahl(3);  // Erhöhe die Anzahl auf dem Feld
                feld->setAnzahl(3);  // Erhöhe die Anzahl auf dem Feld
                spielfeld->splash();  // Zeige das Spielfeld an
            }
    }

	// Rückgabe von allen Feldern, die dem Spieler gehören

	std::vector<Feld*> besetzteFelder(const std::shared_ptr<Spieler>& spieler) {
        std::vector<Feld*> felderDesSpielers;
        for (int i = 0; i < spielfeld->getSize(); i++) {
            for (int j = 0; j < spielfeld->getSize(); j++) {
                Feld& feld = spielfeld->getFeld(i, j);
                if (feld.getOwner() != nullptr && feld.getOwner()->getId() == spieler->getId()) {
                    felderDesSpielers.push_back(&feld);
                }
            }
        }
        return felderDesSpielers;
    }

	// Prüfen ob ein Spieler Felder besitzt

	bool besitztSpielerFelder(const std::shared_ptr<Spieler>& spieler) {
        std::vector<Feld*> felderDesSpielers = besetzteFelder(spieler);
        return !felderDesSpielers.empty();  // Gibt true zurück, wenn der Vektor nicht leer ist
    }

	bool finished() {
		int spielerMitFeldern = 0;
		for (const auto& spieler : spielerVector) {
			if (besitztSpielerFelder(spieler)) {
				spielerMitFeldern++;
			}
		}
		return spielerMitFeldern <= 1; // Gibt true zurück, wenn nur noch ein Spieler oder keiner Felder besitzt
	}

	bool istFarbeVerfuegbar(const Farbe& farbe) {
    for (const auto& spieler : spielerVector) {
        if (spieler->getFarbe() == farbe) {
            return false;  // Farbe ist bereits vergeben
        }
    }
    return true;  // Farbe ist verfügbar
	}	

};
