#ifndef SHIP_H
#define SHIP_H

#include <stdexcept>
#include <vector>
#include <algorithm>

enum class SegmentState { 
    Intact,
    Damaged,
    Destroyed
};

enum class ShipSize {
    SMALL = 1,
    MEDIUM = 2,
    BIG = 3,
    LARGE = 4
};

class Ship {
public:
    Ship(ShipSize length);

    ShipSize getLength() const;
    SegmentState getSegmentState(int index) const;
    void hitSegment(int index);
    bool isDestroyed() const;

private:
    ShipSize length_;
    std::vector<SegmentState> segments_;
};

#endif // SHIP_H