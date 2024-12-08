#ifndef SHIP_SEGMENT_H
#define SHIP_SEGMENT_H

enum class SegmentState {
    Intact, 
    Damaged,
    Destroyed,
};

enum class Attack {
    Default,
    Double,
};

class ShipSegment {
public:
    ShipSegment();
    [[nodiscard]] SegmentState getState() const;
    void hitSegment(Attack attack);
    void setState(SegmentState state);
private:
    SegmentState state_;
};

#endif // SHIP_SEGMENT_H