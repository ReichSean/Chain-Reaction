#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Spiel.cpp"

void zeigeHauptmenue() {
    std::cout << "Hauptmenue:\n";
    std::cout << "1. Neues Spiel starten\n";
    std::cout << "2. Spiel laden\n";
    std::cout << "Waehle eine Option (1-2): ";
}

int main() {
    int auswahl;
    Spiel game;
    zeigeHauptmenue();
    std::cin >> auswahl;

    switch (auswahl) {
        case 1:
            std::cout << "Neues Spielfeld wird erstellt...\n";
            game.spielInitialisieren();
            break;
        case 2:
            std::cout << "Spielstand wird geladen...\n";
            game.spielLaden();
            break;
        default:
            std::cout << "Ungueltige Auswahl!\n";
            return 0;
    }

    char eingabe;
    bool spielLaeuft = true;

    game.getSpielfeld().printSpielfeld();
    while (spielLaeuft) {
        std::cout << "Druecke 'P' um das Spiel zu pausieren und zu speichern, 'Q' um zu beenden oder 'C' um fortzufahren.\n";
        std::cin >> eingabe;

        switch (eingabe) {
            case 'P':
            case 'p':
                game.spielSpeichern();
                std::cout << "Spiel pausiert und gespeichert. Druecke 'C' um fortzufahren.\n";
                break;
            case 'Q':
            case 'q':
                spielLaeuft = false;
                break;
            case 'C':
            case 'c':
                game.spielen();
                spielLaeuft = false;
                break;
            default:
                std::cout << "Ungueltige Eingabe!\n";
                break;
        }
    }

    return 0;
}
