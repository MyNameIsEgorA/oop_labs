#include "ship_segment.h"

ShipSegment::ShipSegment() : state_(SegmentState::Intact) {}

SegmentState ShipSegment::getState() const {
    return this->state_;
}

void ShipSegment::hitSegment() {
    if (this->state_ == SegmentState::Damaged) {
        this->state_ = SegmentState::Destroyed;
        return;
    }
    if (this->state_ == SegmentState::Intact) {
        this->state_ = SegmentState::Damaged;
    }
}