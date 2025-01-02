#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Main.cpp"

class Spiel {
private:
	std::vector<Spieler> spielerVector;
	Spielfeld spielfeld;

public:
	Spiel() : spielfeld(1) {}

	Spiel(Spielfeld& spielfeld) : spielfeld(spielfeld) {}


	void hinzufügenSpieler(Spieler spieler) { spielerVector.push_back(spieler); }
	const Spieler getSpieler(int x) const { return spielerVector[x]; }

	const Spielfeld getSpielfeld() const { return spielfeld; }
	void setSpielfeld(const Spielfeld& sf) { spielfeld = sf; }

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
			spielstand << "Feldgroe�e: " << spielfeld.getSize() << std::endl;
			for (int i = 0; i < spielfeld.getSize(); i++) {
				for (int j = 0; j < spielfeld.getSize(); j++) {
					if (spielfeld.getFeld(i, j).getOwner() == nullptr) {
						spielstand << "Feld: " << i << "," << j << "," << spielfeld.getFeld(i, j).getAnzahl() << ",";
						spielstand << -1 << std::endl;
					}
					else {
						spielstand << "Feld: " << i << "," << j << "," << spielfeld.getFeld(i, j).getAnzahl() << ",";
						spielstand << spielfeld.getFeld(i, j).getOwner()->getId() << std::endl;
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
					setSpielfeld(Spielfeld(zeilen));
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
							spielfeld.getFeld(zeile, spalte).setAnzahl(anzahlPunkte);
							spielfeld.getFeld(zeile, spalte).setOwner(nullptr);
						}
						else {
							for (Spieler& possibleOwner : spielerVector) {
								if (possibleOwner.getId() == ownerId) {
									spielfeld.getFeld(zeile, spalte).setAnzahl(anzahlPunkte);
									spielfeld.getFeld(zeile, spalte).setOwner(&possibleOwner);
									std::cout << "eingetragene Owner id: " << spielfeld.getFeld(zeile, spalte).getOwner()->getId() << std::endl;
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

	
};
