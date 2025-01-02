#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>  // Für system_clock


enum class Farbe {
    Rot, Gruen, Blau, Gelb, Magenta, Cyan, Weiss, Reset
};


class Spieler {
private:

public:
    std::string Name;
    Farbe farbe;
    int id;
    bool isAI;
    int score;
    // Automatische Zuweisungn von id noch unklar !!!
    Spieler(Farbe farbe, const std::string& name, const bool isAI) : Name(name), farbe(farbe), id(generiereZufaelligeID()), isAI(isAI), score(0) {}

    Spieler(Farbe farbe, const std::string& name, const bool isAI, int id) : Name(name), farbe(farbe), id(id), isAI(isAI), score(0) {}

    void setName(const std::string& name) { Name = name; }
    const std::string& getName() const { return Name; }

    void setFarbe(Farbe f) { farbe = f; }
    Farbe getFarbe() const { return farbe; }

    void setId(int i) { id = i; }
    int getId() const { return id; }

    void setScore(int s) { score = s; }
    int getScore() const { return score; }

    void setIsAI(bool b) { isAI = b; }
    int getIsAI() const { return isAI; }

    // Zufällige ID, die im Konstruktor des Spielers erstellt wird
    static int generiereZufaelligeID() {
        static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count()); // Zufallsgenerator
        static std::uniform_int_distribution<int> distribution(1, 10000);
        return distribution(generator);
    }
    //was wenn id meherer Spieler gleich
    //wieso 1-10000
};

class Feld {
private:

public:
    int anzahl;
    Spieler* owner;

    Feld() : anzahl(0), owner(nullptr) {}

    Feld(int anzahl, Spieler* owner) // =0 unnötig
        : anzahl(anzahl), owner(owner) {
    }

    void hinzufuegen() { anzahl++; }

    const Feld& getFeld() const {
        return *this; // Gibt eine konstante Referenz auf das aktuelle Objekt zurück
    }

    void setAnzahl(int num) { anzahl = num; }
    int getAnzahl() const { return anzahl; }

    void setOwner(Spieler* spieler) {
        owner = spieler;
    }

    Spieler* getOwner() const { return owner; }
};

