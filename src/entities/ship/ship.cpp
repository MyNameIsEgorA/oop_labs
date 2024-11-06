#include "ship.h"

Ship::Ship(ShipSize length) : length_(length) {
    segments_.resize(static_cast<int>(length));
    for (auto& segment : segments_) {
        segment = std::make_shared<ShipSegment>();
    }
}

ShipSize Ship::getLength() const {
    return this->length_;
}

std::vector<std::shared_ptr<ShipSegment>> Ship::getSegments() const {
    return this->segments_;
}

void Ship::printShipInfo() const {
    for (const auto& segment : segments_) {
        switch (segment->getState()) {
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
    }
    std::cout << std::endl;
}

bool Ship::isDestroyed() const {
    for (const auto& segment : segments_) {
        if (segment->getState() != SegmentState::Destroyed) {
            return false;
        }
    }
    return true;
}
