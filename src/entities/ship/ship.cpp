#include "ship.h"
#include <algorithm>

Ship::Ship(int length, Orientation orientation) : length_(length), orientation_(orientation) {
    if (length < 1 || length > 4) {
        throw std::invalid_argument("Invalid ship length");
    }
    segments_.resize(length, SegmentState::Intact);
}

void Ship::hitSegment(int segmentIndex) {
    if (segmentIndex < 0 || segmentIndex >= length_) {
        throw std::out_of_range("Invalid segment index");
    }
    if (segments_[segmentIndex] == SegmentState::Intact) {
        segments_[segmentIndex] = SegmentState::Damaged;
    } else if (segments_[segmentIndex] == SegmentState::Damaged) {
        segments_[segmentIndex] = SegmentState::Destroyed;
    }
}

bool Ship::isDestroyed() const {
    return std::all_of(segments_.begin(), segments_.end(), [](SegmentState state) {
        return state == SegmentState::Destroyed;
    });
}

int Ship::getLength() const {
    return length_;
}

Orientation Ship::getOrientation() const {
    return orientation_;
}

SegmentState Ship::getSegmentState(int segmentIndex) const {
    if (segmentIndex < 0 || segmentIndex >= length_) {
        throw std::out_of_range("Invalid segment index");
    }
    return segments_[segmentIndex];
}