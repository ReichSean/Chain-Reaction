
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <chrono>  
#include <thread>
#include "Field.cpp"


class Spielfeld {
private:
    Feld** spielfeld;
    int size;

public:
    Spielfeld(int size) : size(size) {
        spielfeld = new Feld * [size];
        for (int i = 0; i < size; ++i) {
            spielfeld[i] = new Feld[size];
        }
    }

    ~Spielfeld() {
        for (int i = 0; i < size; ++i) {
            delete[] spielfeld[i];
        }
        delete[] spielfeld;
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
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                const Feld& feld = spielfeld[i][j];

                if (feld.getAnzahl() == 0) {
                    // Wenn das Feld leer ist, wei�e Null ausgeben
                    std::cout << getAnsiCode(Farbe::Weiss) << "0" << getAnsiCode(Farbe::Reset) << " ";
                }
                else {
                    // Wenn das Feld einen Besitzer hat, die Besitzerfarbe verwenden
                    if (feld.getOwner()) {
                        std::cout << getAnsiCode(feld.getOwner()->getFarbe()) << feld.getAnzahl() << getAnsiCode(Farbe::Reset) << " ";
                    }
                    else {
                        // Falls kein Besitzer, aber Anzahl > 0, trotzdem Wei� verwenden
                        std::cout << getAnsiCode(Farbe::Weiss) << feld.getAnzahl() << getAnsiCode(Farbe::Reset) << " ";
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    Feld& getFeld(int x, int y) {
        return spielfeld[x][y];
    }

    Feld** getSpielfeld() {
        return spielfeld;
    }

    int getSize() const { return size; }

    void splash() {
        bool explosionOccurred = false;

        std::vector<std::pair<int, int>> explosionPositions;

        printSpielfeld();
        
        //hier am Anfang amfang die Ausgabe löschen (vielleicht)
        //hier nen delay

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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


    //methode score macht hier wenig sinn, da der vector mit spielern im spiel ist, dadr�ber m�ssen wir reden

};

/*int main()
{
    std::cout << "Spielfeld erstellen...\n";

    int size = 5;
    Spielfeld mySpielfeld(size);

    Spieler sean(Farbe::Blau, "Sean", false);
    Spieler jonas(Farbe::Rot, "Jonas", false);



    mySpielfeld.getFeld(2, 2).setOwner(&sean);
    mySpielfeld.getFeld(2, 2).setAnzahl(3);


    mySpielfeld.getFeld(2, 3).setOwner(&sean);
    mySpielfeld.getFeld(2, 3).setAnzahl(3);

    mySpielfeld.getFeld(2, 4).setOwner(&sean);
    mySpielfeld.getFeld(2, 4).setAnzahl(3);


    mySpielfeld.getFeld(1, 1).setOwner(&jonas);
    mySpielfeld.getFeld(1, 1).setAnzahl(2);


    mySpielfeld.printSpielfeld();

    int wait1;
    std::cin >> wait1;


    mySpielfeld.getFeld(2, 2).hinzufuegen();

    mySpielfeld.printSpielfeld();

    int wait2;
    std::cin >> wait2;



    mySpielfeld.splash();

    mySpielfeld.printSpielfeld();

    int wait3;
    std::cin >> wait3;
}*/