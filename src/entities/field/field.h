#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <functional>

#include "../ship/ship.h"
#include "../../managers/ship_manager/ship_manager.h"
#include "../ship_segment/ship_segment.h"
#include "./exceptions.h"

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
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    void placeShip(Ship& ship, int x, int y, Orientation orientation);
    bool attackCell(int x, int y);
    void makePointVisible(int x, int y);
    void printField() const;

private:

    struct Cell {
        std::optional<std::reference_wrapper<ShipSegment>> shipSegment;
        CellState cellState;
    };

    bool checkHorizontalPlacement(int x, int y, ShipSize shipSize);
    bool checkVerticalPlacement(int x, int y, ShipSize shipSize);
    void markAdjacentCells(int x, int y, int length, Orientation orientation);

    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
};