#include "ship.h"

Ship::Ship(ShipSize length) : length_(length) {
    if (length < ShipSize::SMALL || length > ShipSize::LARGE) {
        throw std::invalid_argument("Invalid ship size");
    }
    segments_.resize(static_cast<int>(length), SegmentState::Intact);
}

ShipSize Ship::getLength() const {
    return length_;
}

SegmentState Ship::getSegmentState(int index) const {
    if (index < 0 || index >= static_cast<int>(length_)) {
        throw std::out_of_range("Invalid segment index");
    }
    return segments_[index];
}

void Ship::hitSegment(int index) {
    if (index < 0 || index >= static_cast<int>(length_)) {
        throw std::out_of_range("Invalid segment index");
    }
    if (segments_[index] == SegmentState::Intact) {
        segments_[index] = SegmentState::Damaged;
    } else if (segments_[index] == SegmentState::Damaged) {
        segments_[index] = SegmentState::Destroyed;
    }
}

bool Ship::isDestroyed() const {
    return std::all_of(segments_.begin(), segments_.end(), [](SegmentState state) {
        return state == SegmentState::Destroyed;
    });
}
