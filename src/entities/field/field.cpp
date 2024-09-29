#include "field.h"
#include <iostream>

GameField::GameField(int width, int height) : width_(width), height_(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Invalid field dimensions");
    }
    field_.resize(height, std::vector<CellState>(width, CellState::Unknown));
}

GameField::GameField(const GameField& other) : width_(other.width_), height_(other.height_) {
    field_ = other.field_;
    shipPositions_ = other.shipPositions_;
}

GameField::GameField(GameField&& other) noexcept : width_(other.width_), height_(other.height_), field_(std::move(other.field_)), shipPositions_(std::move(other.shipPositions_)) {
    other.width_ = 0;
    other.height_ = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        field_ = other.field_;
        shipPositions_ = other.shipPositions_;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        field_ = std::move(other.field_);
        shipPositions_ = std::move(other.shipPositions_);
        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

void GameField::placeShip(Ship& ship, int x, int y) {
    if (ship.getOrientation() == Orientation::Horizontal) {
        checkHorizontalPlacement(ship, x, y);
        placeHorizontalShip(ship, x, y);
    } else {
        checkVerticalPlacement(ship, x, y);
        placeVerticalShip(ship, x, y);
    }
    shipPositions_.emplace_back(&ship, x, y);
}

void GameField::attackCell(int x, int y, ShipManager& shipManager) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Invalid cell coordinates");
    }
    if (field_[y][x] == CellState::Ship) {
        for (const auto& pos : shipPositions_) {
            if (pos.ship->getOrientation() == Orientation::Horizontal) {
                if (y == pos.y && x >= pos.x && x < pos.x + pos.ship->getLength()) {
                    pos.ship->hitSegment(x - pos.x);
                    if (pos.ship->getSegmentState(x - pos.x) == SegmentState::Damaged) {
                        field_[y][x] = CellState::Damaged;
                    } else if (pos.ship->getSegmentState(x - pos.x) == SegmentState::Destroyed) {
                        field_[y][x] = CellState::Empty;
                    }
                    if (pos.ship->isDestroyed()) {
                        markShipDestroyed(pos.ship, pos.x, pos.y);
                    }
                    break;
                }
            } else {
                if (x == pos.x && y >= pos.y && y < pos.y + pos.ship->getLength()) {
                    pos.ship->hitSegment(y - pos.y);
                    if (pos.ship->getSegmentState(y - pos.y) == SegmentState::Damaged) {
                        field_[y][x] = CellState::Damaged;
                    } else if (pos.ship->getSegmentState(y - pos.y) == SegmentState::Destroyed) {
                        field_[y][x] = CellState::Empty;
                    }
                    if (pos.ship->isDestroyed()) {
                        markShipDestroyed(pos.ship, pos.x, pos.y);
                    }
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
    std::cout << "\n\n\n";
}

void GameField::checkHorizontalPlacement(const Ship& ship, int x, int y) const {
    if (x + ship.getLength() > width_) {
        throw std::invalid_argument("Ship placement out of bounds");
    }
    for (int i = 0; i < ship.getLength(); ++i) {
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
    if (y + ship.getLength() > height_) {
        throw std::invalid_argument("Ship placement out of bounds");
    }
    for (int i = 0; i < ship.getLength(); ++i) {
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
    for (int i = 0; i < ship.getLength(); ++i) {
        field_[y][x + i] = CellState::Ship;
    }
}

void GameField::placeVerticalShip(const Ship& ship, int x, int y) {
    for (int i = 0; i < ship.getLength(); ++i) {
        field_[y + i][x] = CellState::Ship;
    }
}

void GameField::markShipDestroyed(Ship* ship, int x, int y) {
    if (ship->getOrientation() == Orientation::Horizontal) {
        for (int i = 0; i < ship->getLength(); ++i) {
            field_[y][x + i] = CellState::Empty;
        }
    } else {
        for (int i = 0; i < ship->getLength(); ++i) {
            field_[y + i][x] = CellState::Empty;
        }
    }
}