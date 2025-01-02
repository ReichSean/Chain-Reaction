#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Spiel.cpp"

int main()
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

    Spiel game = Spiel(mySpielfeld);
    game.hinzufügenSpieler(sean);
    game.hinzufügenSpieler(jonas);

    game.spielSpeichern();

    int wait1;
    std::cin >> wait1;


    mySpielfeld.getFeld(2, 2).hinzufuegen();

   

    int wait2;
    std::cin >> wait2;



    mySpielfeld.splash();

   

    int wait3;
    std::cin >> wait3;

    //Spiel newGame = Spiel().spielLaden(); //so geht nicht
    Spiel newGame = game.spielLaden(); //so schon, brauchen aber ja wenn wir frisch starten ein neues SPiel Objekt
    std::cout << "made it this far" << std::endl;

    int wait4;
    std::cin >> wait4;

    newGame.getSpielfeld().printSpielfeld();

    return 0;
}