#include "game.hpp"


int main() {
    Game game;
    while(!game.is_over_) {
        game.makeTurn();
    }
    game.announceWinner();
    game.printBoard();
}
