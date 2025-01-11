#include "Spieler.cpp"




class Feld {
private:
    int anzahl;
    Spieler* owner;
public:

    Feld() : anzahl(0), owner(nullptr) {}

    Feld(int anzahl, Spieler* owner) // =0 unn�tig
        : anzahl(anzahl), owner(owner) {
    }

    void hinzufuegen() { anzahl++; }

    void setAnzahl(int num) { anzahl = num; }
    int getAnzahl() const { return anzahl; }

    void setOwner(Spieler* spieler) {
        owner = spieler;
    }

    Spieler* getOwner() const { return owner; }
};

