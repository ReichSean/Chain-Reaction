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

    game.getSpielfeld().printSpielfeld();
    game.spielen();

    return 0;
}