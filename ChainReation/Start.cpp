#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Spiel.cpp"

void hauptmenue() {
    std::cout << "Willkommen bei ChainReaction!\n";
    std::cout << "1. Neues Spiel starten\n";
    std::cout << "2. Spiel laden\n";
    std::cout << "Waehle eine Option (1-2): ";
}

int main() {
    int auswahl;
    Spiel game;
    hauptmenue();
    std::cin >> auswahl;

    switch (auswahl) {
        case 1:
            std::cout << "Neues Spielfeld wird erstellt...\n";
            game.spielInitialisieren();
            game.spielen();
            break;
        case 2:
            std::cout << "Spielstand wird geladen...\n";
            game.spielLaden();
            game.spielen();
            break;
        default:
            std::cout << "Ungueltige Auswahl!\n";
            return 0;
    }


    return 0;
}
