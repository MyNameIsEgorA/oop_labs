#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <stdexcept>
#include <iostream>

#include "../ship/ship.h"
#include "../../managers/ship_manager/ship_manager.h"

enum class Orientation { 
    Horizontal,
    Vertical
};

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

    void placeShip(Ship& ship, int x, int y, Orientation orientation);
    void attackCell(int x, int y, ShipManager& shipManager);
    void printField() const;

private:
    struct ShipPosition {
        Ship* ship;
        int x, y;
        Orientation orientation;

        ShipPosition(Ship* s, int xPos, int yPos, Orientation o);
    };

    void updateCellState(int x, int y, Ship* ship, int segmentIndex);
    void checkHorizontalPlacement(const Ship& ship, int x, int y) const;
    void checkVerticalPlacement(const Ship& ship, int x, int y) const;
    void placeHorizontalShip(const Ship& ship, int x, int y);
    void placeVerticalShip(const Ship& ship, int x, int y);

    int width_, height_;
    std::vector<std::vector<CellState>> field_;
    std::vector<ShipPosition> shipPositions_;
};

#endif // GAMEFIELD_H