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
	int aktuellerSpielerId;
	bool istGeladen; // maybe noch initialisieren in Konstruktoren

public:
	Spiel() : spielfeld(nullptr) {
	}

	Spiel(std::unique_ptr<Spielfeld> sf) : spielfeld(std::move(sf)) {}

	Spiel& getSpiel() { return *this; }

	void hinzufügenSpieler(const std::shared_ptr<Spieler>& spieler) { spielerVector.push_back(spieler); }

	std::shared_ptr<Spieler> getSpieler(int x) const { return spielerVector[x]; }

	Spielfeld& getSpielfeld() { return *spielfeld; }

	void setSpielfeld(std::unique_ptr<Spielfeld> sf) { spielfeld = std::move(sf); }

	void setAktuellerSpielerId(int spielerId) {
		aktuellerSpielerId = spielerId;
	}

	int getAktuellerSpielerId() const { return aktuellerSpielerId; }

	bool stringToBool(const std::string& s) const {
		return (s == "true" || s == "1");
	}

	std::vector<std::shared_ptr<Spieler>> getSpielerVector() {
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

	std::string getAnsiCode(Farbe farbe) const {
		switch (farbe) {
		case Farbe::Rot: return "\033[31m";
		case Farbe::Gruen: return "\033[32m";
		case Farbe::Blau: return "\033[34m";
		case Farbe::Gelb: return "\033[33m";
		case Farbe::Magenta: return "\033[35m";
		case Farbe::Cyan: return "\033[36m";
		case Farbe::Weiss: return "\033[37m";
		case Farbe::Reset: return "\033[0m";
		default: return "\033[0m";
		}
	}

	void spielSpeichern() {
		std::ofstream spielstand("spielstand.txt");
		if (spielstand.is_open()) {
			//damit der aktuelle Spieler zuerst eingeschrieben wird
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
			//Id des aktiven Spielers zum Zeitpunkt des speicherns speichern
			spielstand << "aktuellerSpielerId: " << getAktuellerSpielerId() << ",";
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
				else if (line.find("aktuellerSpielerId: ") == 0) {
					std::istringstream specStream(line.substr(20));
					if (std::getline(specStream, entry, ',')) {
						setAktuellerSpielerId(std::stoi(entry));
					}
				}
			}
		}
		fReader.close();
		std::cout << "Spielstand erfolgreich geladen!" << std::endl;
		istGeladen = true;
		return;
	}

	bool istDateiLeer() {
		std::ifstream file("spielstand.txt");
		return file.peek() == std::ifstream::traits_type::eof(); // Überprüfen, ob die Datei leer ist
	}

	void spielInitialisieren() {
		spielerVector.clear(); // Entfernt alle Spieler falls neues Spiel erstellt wird

		spielfeld.reset();
		int fieldSizeInput = 0;
		while (fieldSizeInput != 1 && fieldSizeInput != 2) {
			std::cout << "Bitte Spielfeldgroesse waehlen:\n";
			std::cout << "1 - 5x5" << std::endl;
			std::cout << "2 - 7x7" << std::endl;
			std::cin >> fieldSizeInput;
			if (fieldSizeInput == 1) {
				setSpielfeld(std::make_unique<Spielfeld>(5));
			}
			else if (fieldSizeInput == 2) {
				setSpielfeld(std::make_unique<Spielfeld>(7));
			}
			else {
				std::cout << "Ungueltige Eingabe! Bitte erneut eingeben." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Puffer leeren
			}
		}

		std::cout << std::endl; //Abstand zwischen auswahl der Feldgröße und auswahl der Spieleranzahl

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

			int farbenWahl;
			Farbe ausgewaehlteFarbe;

			std::cout << "Bitte waehlen Sie eine Farbe:\n";
			std::cout << "1 - Gruen" << std::endl;
			std::cout << "2 - Blau" << std::endl;
			std::cout << "3 - Gelb" << std::endl;
			std::cout << "4 - Magenta" << std::endl;
			std::cout << "5 - Cyan" << std::endl;

			while (true) {
				std::cin >> farbenWahl;

				// Überprüfen, ob die Eingabe gültig ist
				if (std::cin.fail() || farbenWahl < 1 || farbenWahl > 5) {
					std::cin.clear(); // Fehlerstatus zurücksetzen
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Puffer leeren

					std::cout << "Ungueltige Wahl. Bitte erneut waehlen:\n";
					continue; // Zurück zum Anfang der Schleife
				}



				switch (farbenWahl) {
				case 1:
					ausgewaehlteFarbe = Farbe::Gruen;
					break;
				case 2:
					ausgewaehlteFarbe = Farbe::Blau;
					break;
				case 3:
					ausgewaehlteFarbe = Farbe::Gelb;
					break;
				case 4:
					ausgewaehlteFarbe = Farbe::Magenta;
					break;
				case 5:
					ausgewaehlteFarbe = Farbe::Cyan;
					break;
				default:
					std::cout << "Ungueltige Wahl. Bitte erneut waehlen:\n";
					continue;
				}


				if (!istFarbeVerfuegbar(ausgewaehlteFarbe)) {
					std::cout << "Farbe bereits vergeben. Bitte erneut waehlen:\n";
					continue;
				}

				break; // Gültige Eingabe; Schleife verlassen
			}

			hinzufügenSpieler(std::make_shared<Spieler>(ausgewaehlteFarbe, spielerName, false));
		}

		char KI;
		bool gültigeKIEingabe = false; // maybe ändern und bei allen über so etwas abfragen (bsp gültige Eingabe)

		if (anzahlSpieler == 1) {
			// Wenn nur 1 Spieler, wird der KI-Spieler automatisch hinzugefügt
			std::cout << "Da nur 1 Spieler mitmacht, wird automatisch ein KI-Spieler hinzugefuegt." << std::endl;
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
		istGeladen = false;
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
			return num >= 1 && num <= 5;  // Für 5x5 Spielfeld, Zahlen von 1 bis 5
		}
		else if (size == 7) {
			return num >= 1 && num <= 7;  // Für 7x7 Spielfeld, Zahlen von 1 bis 7
		}
		return false;  // Wenn die Größe unerwartet ist
	}

	bool isValidLetter(char letter) {
		letter = std::toupper(letter);  // Konvertiere Buchstaben in Großbuchstaben
		int size = getSpielfeld().getSize();

		if (size == 5) {
			return letter >= 'A' && letter <= 'E';
		}
		else if (size == 7) {
			return letter >= 'A' && letter <= 'G';
		}
		return false;  // Wenn die Größe unerwartet ist
	}

	std::array<int, 2> getInput(bool gameSaveable) {

		while (true) {
			std::string input;
			std::cin >> input;
			if (input.length() == 2 && isValidLetter(input[0]) && isValidNumber(input[1])) {
				std::array<int, 2> koordinaten;
				koordinaten[0] = static_cast<int>(input[1] - 49); //Weil ASCII bei 48 anfängt und Erste Feld = A1
				koordinaten[1] = letterToNumber(input[0]);
				return koordinaten;
			}
			if ((input[0] == 'q' || input[0] == 'Q') && gameSaveable) {
				pause(getSpiel());
			}
			else {
				std::cout << "Eingabe muss validen Buchstaben und Zahlen innerhalb der Spielgrenzen haben!" << std::endl;
			}
		}
	}

	void pause(Spiel& game) {
		char auswahl;
		bool imMenue = true;
		while (imMenue) {
			std::cout << "Pausemenue:" << std::endl;
			std::cout << "P - Spiel speichern und fortsetzen" << std::endl;
			std::cout << "N - Neues Spiel starten" << std::endl;
			std::cout << "Q - Spiel beenden" << std::endl;
			std::cout << "Waehle eine Option (P/N/Q):" << std::endl;
			std::cin >> auswahl;
			switch (auswahl) {
			case 'P':
			case 'p':
				game.spielSpeichern();
				std::cout << "Spiel gespeichert. Sie koennen weiter spielen" << std::endl;
				imMenue = false;
				break;
			case 'N':
			case 'n':
#ifdef _WIN32
				system("cls");  // Windows
#else
				system("clear");  // Linux/Mac
#endif
				std::cout << "Neues Spiel wird gestartet..." << std::endl;
				game.spielInitialisieren();
				game.spielen();
				imMenue = false;
				break;
			case 'Q':
			case 'q':
				std::cout << "Spiel wird beendet." << std::endl;
				exit(0);
			default:
				std::cout << "Ungültige Eingabe! Bitte wähle P, N oder Q." << std::endl;
				break;
			}
		}
	}

	void spielen() {
		getSpielfeld().splash();
		std::shared_ptr<Spieler> possibleWinner;

		if (!istSpielGeladen()) {
			// Züge am Anfang

			std::random_device rd; // Zufallszahlengenerator initialisieren
			std::mt19937 gen(rd()); // Mersenne-Twister-Generator
			std::shuffle(spielerVector.begin(), spielerVector.end(), gen); // Spieler mischen


			for (size_t i = 0; i < spielerVector.size(); ++i) {
				spielerVector[i]->setId(static_cast<int>(i) + 1); // Setze die ID (1, 2, 3, ...), damit nach dem laden da angefangen wird, wo aufgehört wurde
				ersterZug(spielerVector[i]);
			}
		}
		// Normale Züge
		bool running = true; // maybe noch anders lösen

		//für erste runde nach spiel Laden
		if (istSpielGeladen()) {
			for (auto& spieler : spielerVector) {
				if (spieler->getId() >= getAktuellerSpielerId()) {
					if (!finished()) {
						possibleWinner = spieler;
						setAktuellerSpielerId(spieler->getId());

						if (!spieler->getIsAI()) {  //Bei KI spielern erwarten wir keine eingabe, deshalb auch nicht anzeigen
							std::cout << std::endl;
							std::cout << "Q - Pausenmenue" << std::endl;
							std::cout << std::endl;
						}
						zug(spieler);
					}
					else {
						running = false;
						std::cout << "Spiel zu Ende" << std::endl;
						if (possibleWinner != nullptr) {
							std::cout << "Herzlichen Glueckwunsch, " << possibleWinner->getName() << "! Sie haben gewonnen!" << std::endl;
						}
						exit(0);
						return;
					}
				}
			}
		}
		while (running) {

			for (auto& spieler : spielerVector) {
				if (!finished()) {
					possibleWinner = spieler;
					setAktuellerSpielerId(spieler->getId());
					if (!spieler->getIsAI()) {  //Bei KI spielern erwarten wir keine eingabe, deshalb auch nicht anzeigen
						std::cout << std::endl;
						std::cout << "Q - Pausenmenue" << std::endl;
						std::cout << std::endl;
					}
					zug(spieler);
				}
				else {
					running = false;
					std::cout << "Spiel zu Ende" << std::endl;
					if (possibleWinner != nullptr) {
						std::cout << "Herzlichen Glueckwunsch, " << possibleWinner->getName() << "! Sie haben gewonnen!" << std::endl;
					}
					nachSpielOptionen();
					return;
				}
			}
		}
	}

	void nachSpielOptionen() {
		std::cout << std::endl;
		std::cout << "Moechten Sie eine Revanche (R), das Spiel beenden (B) oder ein neues Spiel starten (N)? ";
		char wahl;
		std::cin >> wahl;

		switch (wahl) {
		case 'R':
		case 'r':
		{
			// Revanche
			int spielfeldGroesse = spielfeld->getSize(); // Größe des aktuellen Spielfeldes speichern
			spielfeld.reset(); // Reset des Spielfeldes
			setSpielfeld(std::make_unique<Spielfeld>(spielfeldGroesse)); // Neues Spielfeld mit alter Größe
			spielen(); // Erneut spielen
			break;
		}
		case 'B':
		case 'b':
		{
			std::cout << std::endl;
			std::cout << "Vielen Dank fürs Spielen! Auf Wiedersehen!" << std::endl;
			exit(0);
			break;
		}
		case 'N':
		case 'n':
		{
#ifdef _WIN32
			system("cls");  // Windows
#else
			system("clear");  // Linux/Mac
#endif
			std::cout << "Neues Spiel wird gestartet..." << std::endl;
			spielInitialisieren(); // Initialisiere ein neues Spiel
			spielen(); // Erneut spielen
			break;
		}
		default:
			std::cout << "Ungültige Auswahl, bitte versuchen Sie es erneut." << std::endl;
			nachSpielOptionen(); // Wiederhole die Optionenauswahl
			break;
		}
	}

	// Zugmethoden für Spieler und KI

	void zug(std::shared_ptr<Spieler>& spieler) {
		if (besitztSpielerFelder(spieler)) {
			if (!spieler->getIsAI()) { // Zug für Nicht-KI Spieler, wenn KI-Spieler, dann KIZug(spieler), also else-Anweisung
				std::cout << getAnsiCode(spieler->getFarbe()) << spieler->getName() << getAnsiCode(Farbe::Reset) << ", bitte waehle ein Feld (z.B. a4, b6, c3 usw.)" << std::endl; //abfrage ob Spiel beenden und speichern

				std::array<int, 2> koordinaten = getInput(true);
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
		if (besitztSpielerFelder(spieler)) {
			std::cout << std::endl;
			std::cout << spieler->getName() << " waehlt ein Feld..." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			Feld* bestesFeld = nullptr;
			int hoechsterScore = -1;

			// Iteriere über alle Felder im Spielfeld, um die Felder zu finden, die dem Spieler gehören
			for (int x = 0; x < spielfeld->getSize(); ++x) {
				for (int y = 0; y < spielfeld->getSize(); ++y) {
					Feld& feld = spielfeld->getFeld(x, y);

					// Überprüfen, ob das Feld dem Spieler gehört
					if (feld.getOwner() == spieler) {
						// Berechne den Score für das aktuelle Feld
						int score = berechneScore(&feld, spieler, x, y); // Koordinaten an die Score-Berechnung übergeben
						if (score > hoechsterScore) {
							hoechsterScore = score;
							bestesFeld = &feld; // Speichere das Feld mit dem höchsten Score
						}
					}
				}
			}

			// Erhöhe das Feld mit dem höchsten Score
			if (bestesFeld != nullptr) {
				bestesFeld->hinzufuegen();
				spielfeld->splash();
			}
		}
		else {
			std::cout << spieler->getName() << " besitzt keine Felder mehr." << std::endl;
		}
	}

	//Hilfsmethoden für KIZug

	int berechneScore(Feld* feld, std::shared_ptr<Spieler> spieler, int x, int y) {
		int score = 0;

		// 1. Punkte für die Anzahl der Kugeln auf dem eigenen Feld
		score += feld->getAnzahl() * 2; // Mehr Punkte für mehr Kugeln

		// 2. Punkte für angrenzende eigene Felder
		for (Feld* nachbar : getAngrenzendeFelder(x, y)) {
			if (nachbar->getOwner() == spieler) {
				score += 2; // Mehr Punkte für angrenzende eigene Felder
			}
		}

		// 3. Bonuspunkte für angrenzende Felder mit 3 Kugeln, wenn eigenes Feld auch 3 kugeln hat

		if (feld->getAnzahl() == 3) {
			for (Feld* nachbar : getAngrenzendeFelder(x, y)) {
				if (nachbar->getAnzahl() == 3) {
					if (nachbar->getOwner() == spieler) {
						score += 5; // Bonus für eigenes angrenzendes Feld mit 3 Kugeln
					}
					else {
						score += 500; // Hoher Bonus für gegnerisches angrenzendes Feld mit 3 Kugeln
					}
				}
			}
		}


		// Abzüge für angrenzende gegnerische Felder mit mehr Kugeln
		for (Feld* nachbar : getAngrenzendeFelder(x, y)) {
			if (nachbar->getOwner() != spieler && nachbar->getAnzahl() > feld->getAnzahl()) {
				if (nachbar->getAnzahl() == 3) {
					score -= 50; //angrenzendes feld gehört Gegner und hat 3 Kugeln 
				}
				else {
					score -= 5; //Gegner feld hat eine Kugel mehr
				}
			}
		}


		// Bonus wenn angrenzende gegnerische Felder mit weniger oder gleich vielen Kugeln
		for (Feld* nachbar : getAngrenzendeFelder(x, y)) {
			if (nachbar->getOwner() != spieler && nachbar->getAnzahl() <= feld->getAnzahl()) {
				score += 5;
			}
		}


		return score;
	}

	std::vector<Feld*> getAngrenzendeFelder(int x, int y) {
		std::vector<Feld*> angrenzendeFelder;

		// Definieren der angrenzenden Positionen
		int benachbarteFelder[4][2] = {
			{x - 1, y}, // Oben
			{x + 1, y}, // Unten
			{x, y - 1}, // Links
			{x, y + 1}  // Rechts
		};

		// Überprüfen der angrenzenden Felder und hinzufügen, wenn gültig
		for (const auto& pos : benachbarteFelder) {
			int nx = pos[0];
			int ny = pos[1];

			// Randwerte checken und nur Felder nehmen, die spielern gehören
			if (nx >= 0 && nx < spielfeld->getSize() && ny >= 0 && ny < spielfeld->getSize()) {
				Feld& nachbarFeld = spielfeld->getFeld(nx, ny);
				if (nachbarFeld.getOwner() != nullptr) { // Überprüfen, ob der Owner nicht null ist
					angrenzendeFelder.push_back(&nachbarFeld);
				}
			}
		}

		return angrenzendeFelder;
	}

	// Erste Zugmethoden für Spieler und KI 

	void ersterZug(std::shared_ptr<Spieler>& spieler) {
		if (!spieler->getIsAI()) { // Erster Zug Methode für nicht KI Spieler, wenn KI ,dann else Anweisung
			std::cout << std::endl;
			std::cout << getAnsiCode(spieler->getFarbe()) << spieler->getName() << getAnsiCode(Farbe::Reset) << ", bitte waehle ein Startfeld (z.B. a4, b6, c3 usw.)" << std::endl;
			std::array<int, 2> koordinaten = getInput(false);
			if (getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).getAnzahl() == 0) {
				getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).setOwner(spieler);
				getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).setAnzahl(3);
				spielfeld->splash();
			}
			else {
				std::cout << "Waehle ein Feld, dass niemandem gehoert!" << std::endl;
				ersterZug(spieler);
			}

		}
		else {
			ersterKIZug(spieler);
		}
	}

	void ersterKIZug(std::shared_ptr<Spieler>& spieler) {
		std::vector<Feld*> freieFelder;
		std::cout << std::endl;
		std::cout << spieler->getName() << " waehlt ein Startfeld..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		for (int x = 1; x < spielfeld->getSize() - 1; x++) {			//nur von 1 bis size-1, weil ein start am rand unvorteilhaft ist
			for (int y = 1; y < spielfeld->getSize() - 1; y++) {
				Feld& feld = spielfeld->getFeld(x, y);

				// Überprüfen, ob das Feld frei ist
				if (feld.getOwner() == nullptr) {
					// Überprüfen, ob angrenzende Felder vom Gegner besetzt sind
					bool nachbarBesetzt = false;
					for (Feld* nachbar : getAngrenzendeFelder(x, y)) {
						if (nachbar->getOwner() != nullptr) {
							nachbarBesetzt = true;
							break;
						}
					}

					if (!nachbarBesetzt) {
						freieFelder.push_back(&feld);
					}
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

	bool istSpielGeladen() const {
		return istGeladen;
	}
};