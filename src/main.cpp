#include <iostream>
#include <limits>
#include "game/game.h"

void clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Game game;
    bool isRunning = true;
    std::string command;

    while (isRunning) {
        game.printGameState();
        
        std::cout << "\nВведите команду (attack x y / ability / exit): ";
        std::cin >> command;

        if (command == "attack") {
            int x, y;
            std::cin >> x >> y;

            if (std::cin.fail()) {
                std::cerr << "Неверный ввод координат" << std::endl;
                clearInputStream();
                continue;
            }

            if (game.makeUserTurn(x, y)) {
                game.makeEnemyTurn();
                
                if (game.isGameOver()) {
                    if (game.hasUserWon()) {
                        std::cout << "\n=== Вы выиграли раунд! ===\n";
                        game.startNewRound();
                    } else {
                        std::cout << "\n=== Вы проиграли! ===\n";
                        std::cout << "Начинаем новую игру...\n";
                        game.startNewGame();
                    }
                }
            }

        } else if (command == "ability") {
            try {
                game.useAbility(); 
                game.makeEnemyTurn();
            } catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }

        } else if (command == "exit") {
            isRunning = false;
        } else {
            std::cerr << "Неизвестная команда" << std::endl;
            clearInputStream();
        }
    }

    std::cout << "Игра завершена!" << std::endl;
    return 0;
}
