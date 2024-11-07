#ifndef SHIP_SEGMENT_H
#define SHIP_SEGMENT_H

enum class SegmentState {
    Intact, 
    Damaged,
    Destroyed,
};

class ShipSegment {
public:
    ShipSegment();
    [[nodiscard]] SegmentState getState() const;
    bool hitSegment();
private:
    SegmentState state_;
};

#endif // SHIP_SEGMENT_H