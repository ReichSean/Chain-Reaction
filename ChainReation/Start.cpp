#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Spiel.cpp"

void hauptmenue(Spiel& game) {
    std::cout << R"(
   ________          _          ____                  __  _           
  / ____/ /_  ____ _(_)___     / __ \___  ____ ______/ /_(_)___  ____ 
 / /   / __ \/ __ `/ / __ \   / /_/ / _ \/ __ `/ ___/ __/ / __ \/ __ \
/ /___/ / / / /_/ / / / / /  / _, _/  __/ /_/ / /__/ /_/ / /_/ / / / /
\____/_/ /_/\__,_/_/_/ /_/  /_/ |_|\___/\__,_/\___/\__/_/\____/_/ /_/ 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    )" << std::endl;;

    if (game.istDateiLeer()) {
        std::cout << "Die Spielstand-Datei ist leer. Ein neues Spiel wird gestartet..." << std::endl;
        std::cout << std::endl;
        game.spielInitialisieren();
        game.spielen();
    }
    else {
        std::cout << "1. Neues Spiel starten" << std::endl;
        std::cout << "2. Spiel laden" << std::endl;
        std::cout << "Waehle eine Option (1-2): " << std::endl;

        int auswahl;
        while (true) {
            std::cin >> auswahl;

            if (std::cin.fail() || auswahl < 1 || auswahl > 2) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Puffer leeren
                std::cout << "Ungueltige Eingabe! Bitte gib 1 oder 2 ein." << std::endl;
                std::cout << "Waehle eine Option (1-2): " << std::endl;
                std::cout << std::endl;
            }
            else {
                break;
            }
        }

        switch (auswahl) {
        case 1:
            std::cout << std::endl;
            std::cout << "Neues Spiel wird erstellt..." << std::endl;
            std::cout << std::endl;
            game.spielInitialisieren();
            game.spielen();
            break;
        case 2:
            std::cout << std::endl;
            std::cout << "Spielstand wird geladen..." << std::endl;
            std::cout << std::endl;
            game.spielLaden();
            game.spielen();
            break;
        }
    }
}

int main() {
    Spiel game;
    hauptmenue(game);
    return 0;
}