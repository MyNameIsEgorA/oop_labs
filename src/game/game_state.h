#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../entities/field/field.h"
#include <vector>
#include <iostream>

class GameState {
public:
    GameState() = default;
    GameState(int width, int height, int round, 
              const Field& userField, const Field& enemyField,
              const std::vector<ShipSize>& userShipSizes,
              const std::vector<ShipSize>& enemyShipSizes,
              const std::vector<int>& abilities);

    friend std::ostream& operator<<(std::ostream& os, const GameState& state);
    friend std::istream& operator>>(std::istream& is, GameState& state);

    int fieldWidth;
    int fieldHeight;
    int currentRound;
    std::vector<std::vector<bool>> userFieldVisible;
    std::vector<std::vector<bool>> enemyFieldVisible;
    std::vector<std::vector<SegmentState>> userFieldState;
    std::vector<std::vector<SegmentState>> enemyFieldState;
    std::vector<ShipSize> userShipSizes;
    std::vector<ShipSize> enemyShipSizes;
    std::vector<int> abilities;
};

#endif 