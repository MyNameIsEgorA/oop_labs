#include "ship_segment.h"

ShipSegment::ShipSegment() : state_(SegmentState::Intact) {}

SegmentState ShipSegment::getState() const {
    return this->state_;
}

void ShipSegment::hitSegment(Attack attack) {
    if (attack == Attack::Double) {
        this->state_ = SegmentState::Damaged;
        return;
    }
    if (this->state_ == SegmentState::Damaged) {
        this->state_ = SegmentState::Destroyed;
        return;
    }
    if (this->state_ == SegmentState::Intact) {
        this->state_ = SegmentState::Damaged;
    }
}

void ShipSegment::setState(SegmentState state) {
    state_ = state;
}