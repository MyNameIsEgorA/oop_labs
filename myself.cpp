#include <iostream>
#include <vector>
#include <stdexcept>

// Enum для состояния сегмента корабля
enum class SegmentState { Intact, // целый
                         Damaged, // поврежденный
                         Destroyed // уничтоженный
};

// Enum для ориентации корабля
enum class Orientation { Horizontal, Vertical };

// Enum для состояния клетки игрового поля
enum class CellState { Unknown, // неизвестно
                       Empty, // пустая
                       Ship, // корабль
                       Damaged // поврежденный сегмент корабля
};

// Класс корабля
class Ship {
public:
    Ship(int length, Orientation orientation) : length_(length), orientation_(orientation) {
        if (length < 1 || length > 4) {
            throw std::invalid_argument("Invalid ship length");
        }
        segments_.resize(length, SegmentState::Intact);
    }

    int getLength() const { return length_; }
    Orientation getOrientation() const { return orientation_; }

    SegmentState getSegmentState(int index) const {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Invalid segment index");
        }
        return segments_[index];
    }

    void hitSegment(int index) {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("Invalid segment index");
        }
        if (segments_[index] == SegmentState::Intact) {
            segments_[index] = SegmentState::Damaged;
        } else if (segments_[index] == SegmentState::Damaged) {
            segments_[index] = SegmentState::Destroyed;
        }
    }

    bool isDestroyed() const {
        for (const auto& segment : segments_) {
            if (segment != SegmentState::Destroyed) {
                return false;
            }
        }
        return true;
    }

private:
    int length_;
    Orientation orientation_;
    std::vector<SegmentState> segments_;
};

// Класс менеджера кораблей
class ShipManager {
public:
    ShipManager(const std::vector<std::pair<int, int>>& shipSpecs) {
        for (const auto& spec : shipSpecs) {
            for (int i = 0; i < spec.second; ++i) {
                ships_.emplace_back(spec.first, Orientation::Horizontal);
            }
        }
    }

    Ship& getShip(int index) {
        if (index < 0 || index >= ships_.size()) {
            throw std::out_of_range("Invalid ship index");
        }
        return ships_[index];
    }

    int getShipCount() const { return ships_.size(); }

    void hitShipSegment(Ship& ship, int segmentIndex) {
        ship.hitSegment(segmentIndex);
    }

private:
    std::vector<Ship> ships_;
};

// Класс игрового поля
class GameField {
public:
    GameField(int width, int height) : width_(width), height_(height) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("Invalid field dimensions");
        }
        field_.resize(height, std::vector<CellState>(width, CellState::Unknown));
    }

    void placeShip(Ship& ship, int x, int y, Orientation orientation) {
        if (orientation == Orientation::Horizontal) {
            checkHorizontalPlacement(ship, x, y);
            placeHorizontalShip(ship, x, y);
        } else {
            checkVerticalPlacement(ship, x, y);
            placeVerticalShip(ship, x, y);
        }
        shipPositions_.emplace_back(&ship, x, y, orientation);
    }

    void attackCell(int x, int y, ShipManager& shipManager) {
        if (x < 0 || x >= width_ || y < 0 || y >= height_) {
            throw std::out_of_range("Invalid cell coordinates");
        }

        if (field_[y][x] == CellState::Ship || field_[y][x] == CellState::Damaged) {
            for (const auto& pos : shipPositions_) {
                if (pos.orientation == Orientation::Horizontal) {
                    if (y == pos.y && x >= pos.x && x < pos.x + pos.ship->getLength()) {
                        int segmentIndex = x - pos.x;
                        shipManager.hitShipSegment(*pos.ship, segmentIndex);
                        updateCellState(x, y, pos.ship, segmentIndex);
                        break;
                    }
                } else {
                    if (x == pos.x && y >= pos.y && y < pos.y + pos.ship->getLength()) {
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

    void printField() const {
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

private:
    struct ShipPosition {
        Ship* ship;
        int x, y;
        Orientation orientation;

        ShipPosition(Ship* s, int xPos, int yPos, Orientation o)
            : ship(s), x(xPos), y(yPos), orientation(o) {}
    };

    void updateCellState(int x, int y, Ship* ship, int segmentIndex) {
        if (ship->getSegmentState(segmentIndex) == SegmentState::Damaged) {
            field_[y][x] = CellState::Damaged;
        } else if (ship->getSegmentState(segmentIndex) == SegmentState::Destroyed) {
            field_[y][x] = CellState::Empty;
            markShipDestroyed(ship, x, y);
        }
    }

    int width_, height_;
    std::vector<std::vector<CellState>> field_;
    std::vector<ShipPosition> shipPositions_;

    void checkHorizontalPlacement(const Ship& ship, int x, int y) const {
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

    void checkVerticalPlacement(const Ship& ship, int x, int y) const {
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

    void placeHorizontalShip(const Ship& ship, int x, int y) {
        for (int i = 0; i < ship.getLength(); ++i) {
            field_[y][x + i] = CellState::Ship;
        }
    }

    void placeVerticalShip(const Ship& ship, int x, int y) {
        for (int i = 0; i < ship.getLength(); ++i) {
            field_[y + i][x] = CellState::Ship;
        }
    }

    void markShipDestroyed(Ship* ship, int x, int y) {
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
};

int main() {
    try {
        ShipManager shipManager({{4, 1}, {3, 2}, {2, 3}, {1, 4}});
        GameField field(10, 10);

        // Размещение кораблей
        // field.placeShip(shipManager.getShip(0), 0, 0, Orientation::Horizontal);
        field.placeShip(shipManager.getShip(1), 2, 0, Orientation::Vertical);
        field.placeShip(shipManager.getShip(2), 4, 4, Orientation::Horizontal);
        // field.placeShip(shipManager.getShip(3), 6, 6, Orientation::Vertical);

        // Атака поля
        field.attackCell(0, 0, shipManager);
        field.attackCell(0, 0, shipManager);
        field.attackCell(1, 0, shipManager);
        field.attackCell(2, 0, shipManager);
        field.attackCell(2, 1, shipManager);
        field.attackCell(2, 0, shipManager);
        field.attackCell(3, 0, shipManager);

        field.printField();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}