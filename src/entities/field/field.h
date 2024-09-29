#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <stdexcept>
#include "../ship/ship.h"
#include "../../managers/ship_manager/ship_manager.h"

enum class CellState {
    Unknown,  
    Empty,    
    Ship,     
    Damaged  
};

class GameField {
public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    void placeShip(Ship& ship, int x, int y);
    void attackCell(int x, int y, ShipManager& shipManager);
    void printField() const;

private:
    int width_;
    int height_;
    std::vector<std::vector<CellState>> field_;
    struct ShipPosition {
        Ship* ship;
        int x, y;
        ShipPosition(Ship* s, int xPos, int yPos) : ship(s), x(xPos), y(yPos) {}
    };
    std::vector<ShipPosition> shipPositions_;

    void checkHorizontalPlacement(const Ship& ship, int x, int y) const;
    void checkVerticalPlacement(const Ship& ship, int x, int y) const;
    void placeHorizontalShip(const Ship& ship, int x, int y);
    void placeVerticalShip(const Ship& ship, int x, int y);
    void markShipDestroyed(Ship* ship, int x, int y);
};

#endif // GAMEFIELD_H