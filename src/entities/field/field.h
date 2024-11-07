#pragma once

#include <vector>
#include <optional>
#include <functional>

#include "../ship/ship.h"
#include "./exceptions.h"
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
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    void placeShip(Ship& ship, int x, int y, Orientation orientation);
    bool attackCell(int x, int y);
    void makePointVisible(int x, int y);
    void printField() const;
    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

private:

    struct Cell {
        std::optional<std::reference_wrapper<ShipSegment>> shipSegment;
        CellState cellState;
    };

    [[nodiscard]] bool checkHorizontalPlacement(int x, int y, ShipSize shipSize) const;
    [[nodiscard]] bool checkVerticalPlacement(int x, int y, ShipSize shipSize) const;

    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
};