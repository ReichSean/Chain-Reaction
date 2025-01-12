#include<memory>
#include "Spieler.cpp"




class Feld {
private:
    int anzahl;
    std::shared_ptr<Spieler> owner;
public:

    Feld() : anzahl(0), owner(nullptr) {}

    Feld(int anzahl, std::shared_ptr<Spieler> owner) // =0 unn�tig
        : anzahl(anzahl), owner(owner) {
    }

    void hinzufuegen() { anzahl++; }

    void setAnzahl(int num) { anzahl = num; }
    int getAnzahl() const { return anzahl; }

    void setOwner(std::shared_ptr<Spieler> spieler) {
        owner = spieler; // Zuweisung eines shared_ptr
    }

    std::shared_ptr<Spieler> getOwner() const { return owner; }
};

