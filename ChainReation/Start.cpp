#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Spiel.cpp"

int main()
{
    std::cout << "Spielfeld erstellen...\n";
    

    Spiel game = Spiel();

    game.setSpielfeld(new Spielfeld(5));

    Spieler sean(Farbe::Blau, "Sean", false);
    Spieler jonas(Farbe::Rot, "Jonas", false);

    game.hinzufügenSpieler(sean);
    game.hinzufügenSpieler(jonas);

    game.getSpielfeld().getFeld(2, 2).setOwner(&sean);
    game.getSpielfeld().getFeld(2, 2).setAnzahl(3);

    game.getSpielfeld().getFeld(2, 3).setOwner(&sean);
    game.getSpielfeld().getFeld(2, 3).setAnzahl(3);

    game.getSpielfeld().getFeld(2, 4).setOwner(&sean);
    game.getSpielfeld().getFeld(2, 4).setAnzahl(3);

    game.getSpielfeld().getFeld(1, 1).setOwner(&jonas);
    game.getSpielfeld().getFeld(1, 1).setAnzahl(2);

    game.getSpielfeld().printSpielfeld();
    
    game.spielSpeichern();

    game.getSpielfeld().getFeld(2, 2).hinzufuegen();

    int wait2;
    std::cin >> wait2;

    game.getSpielfeld().splash();

   

    int wait3;
    std::cin >> wait3;

    Spiel newGame = game.spielLaden(); //so schon, brauchen aber ja wenn wir frisch starten ein neues SPiel Objekt
    std::cout << "made it this far" << std::endl;

    int wait4;
    std::cin >> wait4;

    newGame.getSpielfeld().printSpielfeld();

    return 0;
}