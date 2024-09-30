#include "field.h"

GameField::GameField(int width, int height) : width_(width), height_(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Invalid field dimensions");
    }
    field_.resize(height, std::vector<CellState>(width, CellState::Unknown));
}

GameField::GameField(const GameField& other) 
    : width_(other.width_), height_(other.height_), field_(other.field_), shipPositions_(other.shipPositions_) {
    for (auto& pos : shipPositions_) {
        pos.ship = new Ship(*pos.ship);
    }
}

GameField::GameField(GameField&& other) noexcept
    : width_(other.width_), height_(other.height_), field_(std::move(other.field_)), shipPositions_(std::move(other.shipPositions_)) {
    for (auto& pos : shipPositions_) {
        pos.ship = other.shipPositions_[&pos - &shipPositions_[0]].ship;
        other.shipPositions_[&pos - &shipPositions_[0]].ship = nullptr;
    }
    other.width_ = 0;
    other.height_ = 0;
}

void GameField::placeShip(Ship& ship, int x, int y, Orientation orientation) {
    if (orientation == Orientation::Horizontal) {
        checkHorizontalPlacement(ship, x, y);
        placeHorizontalShip(ship, x, y);
    } else {
        checkVerticalPlacement(ship, x, y);
        placeVerticalShip(ship, x, y);
    }
    shipPositions_.emplace_back(&ship, x, y, orientation);
}

void GameField::attackCell(int x, int y, ShipManager& shipManager) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Invalid cell coordinates");
    }

    if (field_[y][x] == CellState::Ship || field_[y][x] == CellState::Damaged) {
        for (const auto& pos : shipPositions_) {
            if (pos.orientation == Orientation::Horizontal) {
                if (y == pos.y && x >= pos.x && x < pos.x + static_cast<int>(pos.ship->getLength())) {
                    int segmentIndex = x - pos.x;
                    shipManager.hitShipSegment(*pos.ship, segmentIndex);
                    updateCellState(x, y, pos.ship, segmentIndex);
                    break;
                }
            } else {
                if (x == pos.x && y >= pos.y && y < pos.y + static_cast<int>(pos.ship->getLength())) {
                    int segmentIndex = y - pos.y;
                    shipManager.hitShipSegment(*pos.ship, segmentIndex);
                    updateCellState(x, y, pos.ship, segmentIndex);
                    break;
                }
            }
        }
    } else if (field_[y][x] == CellState::Unknown) {
        field_[y][x] = CellState::Empty;
    }
}

void GameField::printField() const {
    for (const auto& row : field_) {
        for (const auto& cell : row) {
            switch (cell) {
                case CellState::Unknown:
                    std::cout << "?";
                    break;
                case CellState::Empty:
                    std::cout << ".";
                    break;
                case CellState::Ship:
                    std::cout << "#";
                    break;
                case CellState::Damaged:
                    std::cout << "*";
                    break;
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

GameField::ShipPosition::ShipPosition(Ship* s, int xPos, int yPos, Orientation o)
    : ship(s), x(xPos), y(yPos), orientation(o) {}

void GameField::updateCellState(int x, int y, Ship* ship, int segmentIndex) {
    if (ship->getSegmentState(segmentIndex) == SegmentState::Damaged) {
        field_[y][x] = CellState::Damaged;
    } else if (ship->getSegmentState(segmentIndex) == SegmentState::Destroyed) {
        field_[y][x] = CellState::Empty;
    }
}

void GameField::checkHorizontalPlacement(const Ship& ship, int x, int y) const {
    if (x + static_cast<int>(ship.getLength()) > width_) {
        throw std::invalid_argument("Ship placement out of bounds");
    }
    for (int i = 0; i < static_cast<int>(ship.getLength()); ++i) {
        if (field_[y][x + i] != CellState::Unknown) {
            throw std::invalid_argument("Ship placement overlaps with another ship");
        }
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (y + dy >= 0 && y + dy < height_ && x + i + dx >= 0 && x + i + dx < width_) {
                    if (field_[y + dy][x + i + dx] == CellState::Ship) {
                        throw std::invalid_argument("Ship placement too close to another ship");
                    }
                }
            }
        }
    }
}

void GameField::checkVerticalPlacement(const Ship& ship, int x, int y) const {
    if (y + static_cast<int>(ship.getLength()) > height_) {
        throw std::invalid_argument("Ship placement out of bounds");
    }
    for (int i = 0; i < static_cast<int>(ship.getLength()); ++i) {
        if (field_[y + i][x] != CellState::Unknown) {
            throw std::invalid_argument("Ship placement overlaps with another ship");
        }
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (y + i + dy >= 0 && y + i + dy < height_ && x + dx >= 0 && x + dx < width_) {
                    if (field_[y + i + dy][x + dx] == CellState::Ship) {
                        throw std::invalid_argument("Ship placement too close to another ship");
                    }
                }
            }
        }
    }
}

void GameField::placeHorizontalShip(const Ship& ship, int x, int y) {
    for (int i = 0; i < static_cast<int>(ship.getLength()); ++i) {
        field_[y][x + i] = CellState::Ship;
    }
}

void GameField::placeVerticalShip(const Ship& ship, int x, int y) {
    for (int i = 0; i < static_cast<int>(ship.getLength()); ++i) {
        field_[y + i][x] = CellState::Ship;
    }
}