#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <stdexcept>

enum class SegmentState {
    Intact,
    Damaged,  
    Destroyed  
};

enum class Orientation {
    Horizontal,
    Vertical
};

class Ship {
public:
    Ship(int length, Orientation orientation);
    void hitSegment(int segmentIndex);
    bool isDestroyed() const;
    int getLength() const;
    Orientation getOrientation() const;
    SegmentState getSegmentState(int segmentIndex) const;

private:
    int length_;
    Orientation orientation_;
    std::vector<SegmentState> segments_;
};

#endif // SHIP_H