#include <string>
#include "Farbe.cpp"



class Spieler {
private:
    std::string Name;
    Farbe farbe;
    int id;
    bool isAI;
    int score;
public:
    Spieler(Farbe farbe, const std::string& name, const bool isAI) : Name(name), farbe(farbe), id(generiereNaechsteID()), isAI(isAI), score(0) {}

    Spieler(Farbe farbe, const std::string& name, const bool isAI, int id) : Name(name), farbe(farbe), id(id), isAI(isAI), score(0) {}

    static int generiereNaechsteID() {
        static int naechsteID = 1;  // Startet bei 1 und wird bei jedem Aufruf erhöht
        return naechsteID++;
    }

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
};