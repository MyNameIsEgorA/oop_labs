#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <iostream>
#include <memory>


#include "../ship_segment/ship_segment.h"


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
    bool isDestroyed() const;
    std::vector<std::shared_ptr<ShipSegment>> getSegments() const;
    void printShipInfo() const;
private:
    ShipSize length_;
    std::vector<std::shared_ptr<ShipSegment>> segments_;
};


#endif // SHIP_H