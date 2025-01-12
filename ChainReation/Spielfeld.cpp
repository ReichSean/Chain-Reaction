#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <chrono>  
#include <thread>
#include <memory>
#include "Feld.cpp"


class Spielfeld {
private:
    std::vector<std::vector<std::unique_ptr<Feld>>> spielfeld;
    int size;

public:
    Spielfeld(int size) : size(size) {
        spielfeld.resize(size);
        for (int i = 0; i < size; ++i) {
            spielfeld[i].resize(size);
            for (int j = 0; j < size; ++j) {
                spielfeld[i][j] = std::make_unique<Feld>();
            }
        }
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

    void printSpielfeld() const {
        #ifdef _WIN32
            system("cls");  // Windows
        #else
            system("clear");  // Linux/Mac
        #endif
        std::cout << getAnsiCode(Farbe::Blau) << "   "; 
        for (int j = 0; j < size; ++j) {
            char columnLabel = 'A' + j; 
            std::cout << getAnsiCode(Farbe::Blau) << columnLabel << " " << getAnsiCode(Farbe::Reset);
        }
        std::cout << std::endl;

        for (int i = 0; i < size; ++i) {
            std::cout << getAnsiCode(Farbe::Blau) << (i + 1) << " "; 

            if (i + 1 < 10) {
                std::cout << " "; //Platz, weil 10 länger als 1-9
            }

            for (int j = 0; j < size; ++j) {
                const Feld& feld = *spielfeld[i][j];

                if (feld.getAnzahl() == 0) {
                    // Wenn das Feld leer ist, weiße Null ausgeben
                    std::cout << getAnsiCode(Farbe::Weiss) << "0" << getAnsiCode(Farbe::Reset) << " ";
                }
                else {
                    // Wenn das Feld einen Besitzer hat, die Besitzerfarbe verwenden
                    if (feld.getOwner()) {
                        std::cout << getAnsiCode(feld.getOwner()->getFarbe()) << feld.getAnzahl() << getAnsiCode(Farbe::Reset) << " ";
                    }
                    else {
                        // Falls kein Besitzer, aber Anzahl > 0, trotzdem Weiß verwenden
                        std::cout << getAnsiCode(Farbe::Weiss) << feld.getAnzahl() << getAnsiCode(Farbe::Reset) << " ";
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    Feld& getFeld(int x, int y) {
        return *spielfeld[x][y];
    }

    const std::vector<std::vector<std::unique_ptr<Feld>>>& getSpielfeld() const {
        return spielfeld;
    }

    int getSize() const { return size; }

    void splash() {
        bool explosionOccurred = false;

        std::vector<std::pair<int, int>> explosionPositions;

        printSpielfeld();
        score();
       

        std::this_thread::sleep_for(std::chrono::milliseconds(800));

        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                Feld& feld = getFeld(x, y);

                if (feld.getAnzahl() > 3) {
                    explosionPositions.emplace_back(x, y); // Speichere (x, y) als Paar
                    explosionOccurred = true;
                }
            }
        }

        for (const auto& pos : explosionPositions) {
            explosion(pos.first, pos.second);
        }

        if (explosionOccurred) {
            splash(); 
        }
    }

    void explosion(int x, int y) {
        Feld& feld = getFeld(x, y);

        if (feld.getAnzahl() <= 3) {
            return;
        }

        int überschüssigeKugeln = feld.getAnzahl() - 3;

        feld.setAnzahl(0);


        int benachbarteFelder[4][2] = {
            {x - 1, y},
            {x + 1, y},
            {x, y - 1},
            {x, y + 1}
        };

        for (int i = 0; i < 4; ++i) {
            int nx = benachbarteFelder[i][0];
            int ny = benachbarteFelder[i][1];

            if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
                Feld& benachbartesFeld = getFeld(nx, ny);
                benachbartesFeld.setAnzahl(benachbartesFeld.getAnzahl()+überschüssigeKugeln);
                benachbartesFeld.setOwner(feld.getOwner());
            }
        }

        feld.setOwner(nullptr);
    }

    void score() {
        // Map zur Speicherung von Spielern und deren Scores
        std::map<std::shared_ptr<Spieler>, int> scores;

        // Alle Felder durchgehen
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                Feld& feld = getFeld(x, y);
                std::shared_ptr<Spieler> owner = feld.getOwner();

                // Felder ohne Besitzer ignorieren
                if (owner == nullptr) continue;

                // Score des Spielers um 1 erhöhen
                scores[owner]++;
            }
        }

        // Scores ausgeben
        for (const auto& entry : scores) {
            std::shared_ptr<Spieler> player = entry.first;
            int score = entry.second;

            // Ausgabe mit Spielerfarbe
            std::cout << getAnsiCode(player->getFarbe())
                << player->getName() << ": " << score
                << getAnsiCode(Farbe::Reset) << std::endl;
        }
    }

};
