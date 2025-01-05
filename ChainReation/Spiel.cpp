#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include "Main.cpp"

class Spiel {
private:
	std::vector<Spieler> spielerVector;
	Spielfeld* spielfeld;

public:
	Spiel() : spielfeld(nullptr) {
		spielInitialisieren();
	}

	Spiel(Spielfeld* spielfeld) : spielfeld(spielfeld) {}
	

	void hinzufügenSpieler(const Spieler& spieler) { spielerVector.push_back(spieler); }
	const Spieler getSpieler(int x) const { return spielerVector[x]; }

	Spielfeld& getSpielfeld() { return *spielfeld; }
	void setSpielfeld(Spielfeld* sf) { spielfeld = sf; }

	bool stringToBool(const std::string& s) const {
		return (s == "true");
	}

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
			for (const Spieler& spieler : spielerVector) {
				spielstand << "Spieler: " << spieler.getId() << "," << spieler.getName() << "," << spieler.getIsAI() << ",";
				spielstand << enumToString(spieler.getFarbe()) << "," << spieler.getScore() << std::endl; //hier Frage ob Score speichern oder sp�ter einfach berechnen aus den feldern? + Frage wegen Enum Farbe obs so passt
			}
			spielstand << "Feldgroe�e: " << getSpielfeld().getSize() << std::endl;
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
			std::cerr << "Fehler beim öffnen der Datei!" << std::endl;
		}
	}

	Spiel spielLaden() {
		std::ifstream fReader("spielstand.txt");
		std::string line;
		if (!fReader.is_open()) {
			std::cout << "Datei konnte nicht geoeffnet werden!" << std::endl;

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
					hinzufügenSpieler(Spieler(farbe, name, isAI, spielerId));
				}
				else if (line.find("Feldgroesse: ") == 0) {
					std::istringstream specStream(line.substr(12));
					int zeilen;
					if (std::getline(specStream, entry, ',')) {
						zeilen = std::stoi(entry);
						std::cout << "gelesene SpielfeldZeilen: " << zeilen << std::endl;
					}
					setSpielfeld(new Spielfeld(zeilen));
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
							for (Spieler& possibleOwner : spielerVector) {
								if (possibleOwner.getId() == ownerId) {
									getSpielfeld().getFeld(zeile, spalte).setAnzahl(anzahlPunkte);
									getSpielfeld().getFeld(zeile, spalte).setOwner(&possibleOwner);
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
		return *this; //hier knallts somehow bei Spiel().spielLaden()
	}

	void spielInitialisieren() {
		int anzahlSpieler;
		std::cout << "Wie viele Spieler machen mit?" << std::endl;
		//checken dass Angabe zwischen 0 und 4 liegt
		std::cin >> anzahlSpieler;
		if(anzahlSpieler <= 2){
			setSpielfeld(new Spielfeld(5));
		}
		else {
			setSpielfeld(new Spielfeld(10));
		}
		for (int i = 0; i < anzahlSpieler; i++) {
			std::string spielerName;
			std::string spielerFarbe;
			std::cout << "-------------------" << std::endl;
			std::cout << "Bitte geben Sie einen Namen ein:" << std::endl;
			std::cin >> spielerName;
			std::cout << "Bitte geben Sie eine Farbe an:" << std::endl;
			std::cin >> spielerFarbe;
			//checken das nicht Farben doppelt vergeben werden
			hinzufügenSpieler(Spieler(stringToEnum(spielerFarbe), spielerName, false));
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

	std::array<int, 2> getInput() {
		std::cout << "Bitte waehle ein Feld" << std::endl;
		std::string input;
		std::cin >> input;
		if (input.length() == 2 && std::isalpha(input[0]) && std::isdigit(input[1])) {
			char firstChar = input[0];
			char secondChar = input[1];
			std::array<int, 2> koordinaten;
			koordinaten[0] = letterToNumber(input[0]);
			koordinaten[1] = static_cast<int>(input[1]-48); //Weil ASCII bei 48 anfängt
			return koordinaten;
		}
		else {
			std::cout << "Die Eingabe muss genau 2 Zeichen lang sein!" << std::endl;
			getInput();
		}
	}
	
	void spielen() {
		
		for (Spieler spieler : spielerVector) {
			ersterZug(spieler);
		}

		//zug();
	}

	void zug(Spieler spieler) {
		getSpielfeld().printSpielfeld();

	}

	void ersterZug(Spieler& spieler) {
		std::array<int, 2> koordinaten = getInput();
		if (getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).getAnzahl() == 0) {
			getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).setOwner(&spieler);
			getSpielfeld().getFeld(koordinaten[0], koordinaten[1]).hinzufuegen();
			getSpielfeld().printSpielfeld();
		}
		else {
			std::cout << "Waehle ein Feld, dass niemandem gehoert!" << std::endl;
			ersterZug(spieler);
		}

	}
	
	
};
