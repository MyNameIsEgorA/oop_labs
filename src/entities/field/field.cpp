#include "field.h"
#include <ncurses.h>


Field::Field(int width, int height) : width_(width), height_(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Invalid field dimensions");
    }
    grid_.resize(height, std::vector<Cell>(width, {std::nullopt, CellState::Invisible}));
}

void Field::placeShip(Ship& ship, int x, int y, Orientation orientation) {
    int length = static_cast<int>(ship.getLength());
    if (orientation == Orientation::Horizontal) {
        if (!checkHorizontalPlacement(x, y, ship.getLength())) {
            throw std::invalid_argument("Ship placement out of bounds or overlapping another ship");
        }
        for (int i = 0; i < length; ++i) {
            grid_[y][x + i].shipSegment = std::ref(*ship.getSegments()[i]);
        }
    } else {
        if (!checkVerticalPlacement(x, y, ship.getLength())) {
            throw std::invalid_argument("Ship placement out of bounds or overlapping another ship");
        }
        for (int i = 0; i < length; ++i) {
            grid_[y + i][x].shipSegment = std::ref(*ship.getSegments()[i]);
        }
    }
}

bool Field::checkHorizontalPlacement(int x, int y, ShipSize shipSize) {
    int length = static_cast<int>(shipSize);
    if (x + length > width_) {
        return false;
    }
    for (int i = 0; i < length; ++i) {
        if (grid_[y][x + i].shipSegment.has_value()) {
            return false;
        }
    }
    return true;
}

bool Field::checkVerticalPlacement(int x, int y, ShipSize shipSize) {
    int length = static_cast<int>(shipSize);
    if (y + length > height_) {
        return false;
    }
    for (int i = 0; i < length; ++i) {
        if (grid_[y + i][x].shipSegment.has_value()) {
            return false;
        }
    }
    return true;
}

void Field::attackCell(int x, int y) {
    if (grid_[y][x].shipSegment.has_value()) {
        grid_[y][x].shipSegment->get().hitSegment();
        makePointVisible(x, y);
        std::cout << "Hit!" << std::endl;
    } else {
        makePointVisible(x, y);
        std::cout << "Missed!" << std::endl;
    }
}

void Field::makePointVisible(int x, int y) {
    grid_[y][x].cellState = CellState::Visible;
}

void Field::printField() const {
    for (const auto& row : grid_) {
        for (const auto& cell : row) {
            if (cell.cellState == CellState::Invisible) {
                std::cout << "\033[37m\u25A1\033[0m" << " "; // белый квадрат
            } else {
                if (cell.shipSegment.has_value()) {
                    switch (cell.shipSegment->get().getState()) {
                        case SegmentState::Destroyed:
                            std::cout << "\033[91m\u25A0\033[0m" << " "; // Красный квадрат
                            break;
                        case SegmentState::Intact:
                            std::cout << "\033[92m\u25A0\033[0m" << " "; // Зеленый квадрат
                            break;
                        case SegmentState::Damaged:
                            std::cout << "\033[93m\u25A0\033[0m" << " "; // Желтый квадрат
                            break;
                        default:
                            break;
                    }
                } else {
                    std::cout << "\033[90m\u25A1\033[0m" << " "; // Серый квадрат
                }
            }
        }
        std::cout << std::endl;
    }
}
