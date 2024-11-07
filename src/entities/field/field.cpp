#include "field.h"

Field::Field(int width, int height) : width_(width), height_(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Invalid field dimensions");
    }
    grid_.resize(height, std::vector<Cell>(width, {std::nullopt, CellState::Invisible}));
}

Field::Field(const Field &other)
    : width_(other.width_), height_(other.height_), grid_(other.grid_) {
}

Field::Field(Field &&other) noexcept
    : width_(other.width_), height_(other.height_), grid_(std::move(other.grid_)) {
}

Field &Field::operator=(const Field &other) {
    if (this != &other) {
        grid_.clear();

        width_ = other.width_;
        height_ = other.height_;
        grid_ = other.grid_;
    }
    return *this;
}

Field &Field::operator=(Field &&other) noexcept {
    if (this != &other) {
        grid_.clear();

        width_ = other.width_;
        height_ = other.height_;
        grid_ = std::move(other.grid_);
    }
    return *this;
}

void Field::placeShip(Ship &ship, int x, int y, Orientation orientation) {
    if (x < 0 || y < 0 || x >= this->width_ || y >= this->height_) {
        throw ShipPlacementOutOfRangeException();
    }

    int length = static_cast<int>(ship.getLength());

    if (orientation == Orientation::Horizontal) {
        if (!checkHorizontalPlacement(x, y, ship.getLength())) {
            throw ShipIncorrectPlacementException();
        }
        for (int i = 0; i < length; ++i) {
            grid_[y][x + i].shipSegment = std::ref(*ship.getSegments()[i]);
        }
    } else {
        if (!checkVerticalPlacement(x, y, ship.getLength())) {
            throw ShipIncorrectPlacementException();
        }
        for (int i = 0; i < length; ++i) {
            grid_[y + i][x].shipSegment = std::ref(*ship.getSegments()[i]);
        }
    }
}

int Field::getHeight() const {
    return this->height_;
}

int Field::getWidth() const {
    return this->width_;
}

bool Field::checkHorizontalPlacement(int x, int y, ShipSize shipSize) const {
    int length = static_cast<int>(shipSize);
    if (x + length > width_) {
        return false;
    }
    for (int i = 0; i < length; ++i) {
        if (grid_[y][x + i].shipSegment.has_value()) {
            return false;
        }

        if (y > 0 && grid_[y - 1][x + i].shipSegment.has_value()) return false;
        if (y < height_ - 1 && grid_[y + 1][x + i].shipSegment.has_value()) return false;
    }

    if (y > 0 && x > 0 && grid_[y - 1][x - 1].shipSegment.has_value()) return false;
    if (y > 0 && x + length < width_ && grid_[y - 1][x + length].shipSegment.has_value()) return false;
    if (y < height_ - 1 && x > 0 && grid_[y + 1][x - 1].shipSegment.has_value()) return false;
    if (y < height_ - 1 && x + length < width_ && grid_[y + 1][x + length].shipSegment.has_value()) return false;
    return true;
}

bool Field::checkVerticalPlacement(int x, int y, ShipSize shipSize) const {
    int length = static_cast<int>(shipSize);
    if (y + length > height_) {
        return false;
    }
    for (int i = 0; i < length; ++i) {
        if (grid_[y + i][x].shipSegment.has_value()) {
            return false;
        }

        if (x > 0 && grid_[y + i][x - 1].shipSegment.has_value()) return false;
        if (x < width_ - 1 && grid_[y + i][x + 1].shipSegment.has_value()) return false;
    }

    if (x > 0 && y > 0 && grid_[y - 1][x - 1].shipSegment.has_value()) return false;
    if (x > 0 && y + length < height_ && grid_[y + length][x - 1].shipSegment.has_value()) return false;
    if (x < width_ - 1 && y > 0 && grid_[y - 1][x + 1].shipSegment.has_value()) return false;
    if (x < width_ - 1 && y + length < height_ && grid_[y + length][x + 1].shipSegment.has_value()) return false;
    return true;
}

bool Field::attackCell(int x, int y) {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        throw AttackOutOfRangeException();
    }

    if (grid_[y][x].shipSegment.has_value()) {
        bool wasDestroyed = grid_[y][x].shipSegment->get().hitSegment();
        makePointVisible(x, y);
        return wasDestroyed;
    } else {
        makePointVisible(x, y);
        return false;
    }
}

void Field::makePointVisible(int x, int y) {
    grid_[y][x].cellState = CellState::Visible;
}

void Field::printField() const {
    for (const auto &row: grid_) {
        for (const auto &cell: row) {
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
