#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <functional>

#include "../ship/ship.h"
#include "../../managers/ship_manager/ship_manager.h"
#include "../ship_segment/ship_segment.h"

enum class Orientation {
    Horizontal, 
    Vertical
};

enum class CellState {
    Visible,
    Invisible,
};

class Field {
public: 
    Field(int width, int height);

    void placeShip(Ship& ship, int x, int y, Orientation orientation);
    void attackCell(int x, int y);
    void makePointVisible(int x, int y);
    void printField() const;

private:

    struct Cell {
        std::optional<std::reference_wrapper<ShipSegment>> shipSegment;
        CellState cellState;
    };

    bool checkHorizontalPlacement(int x, int y, ShipSize shipSize);
    bool checkVerticalPlacement(int x, int y, ShipSize shipSize);

    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
};