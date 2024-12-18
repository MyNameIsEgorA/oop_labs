#include "game_state.h"

GameState::GameState(const int width, const int height, const int round,
                     const Field& userField, const Field& enemyField,
                     const std::vector<ShipSize>& userShips,
                     const std::vector<ShipSize>& enemyShips)
    : fieldWidth(width)
    , fieldHeight(height)
    , currentRound(round)
    , userShipSizes(userShips)
    , enemyShipSizes(enemyShips) {
    
    userFieldVisible.resize(height, std::vector<bool>(width));
    enemyFieldVisible.resize(height, std::vector<bool>(width));
    userFieldState.resize(height, std::vector<SegmentState>(width));
    enemyFieldState.resize(height, std::vector<SegmentState>(width));


    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            userFieldVisible[y][x] = userField.isCellVisible(x, y);
            enemyFieldVisible[y][x] = enemyField.isCellVisible(x, y);
            userFieldState[y][x] = userField.getCellState(x, y);
            enemyFieldState[y][x] = enemyField.getCellState(x, y);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const GameState& state) {
    os << state.fieldWidth << ' ' << state.fieldHeight << ' ' << state.currentRound << '\n';

    for (int y = 0; y < state.fieldHeight; ++y) {
        for (int x = 0; x < state.fieldWidth; ++x) {
            os << static_cast<int>(state.userFieldVisible[y][x]) << ' ';
            os << static_cast<int>(state.userFieldState[y][x]) << ' ';
        }
        os << '\n';
    }
    
    for (int y = 0; y < state.fieldHeight; ++y) {
        for (int x = 0; x < state.fieldWidth; ++x) {
            os << static_cast<int>(state.enemyFieldVisible[y][x]) << ' ';
            os << static_cast<int>(state.enemyFieldState[y][x]) << ' ';
        }
        os << '\n';
    }

    os << state.userShipSizes.size() << '\n';
    for (const auto& size : state.userShipSizes) {
        os << static_cast<int>(size) << ' ';
    }
    os << '\n';
    
    os << state.enemyShipSizes.size() << '\n';
    for (const auto& size : state.enemyShipSizes) {
        os << static_cast<int>(size) << ' ';
    }
    os << '\n';
    
    return os;
}

std::istream& operator>>(std::istream& is, GameState& state) {
    is >> state.fieldWidth >> state.fieldHeight >> state.currentRound;
    
    state.userFieldVisible.resize(state.fieldHeight, std::vector<bool>(state.fieldWidth));
    state.enemyFieldVisible.resize(state.fieldHeight, std::vector<bool>(state.fieldWidth));
    state.userFieldState.resize(state.fieldHeight, std::vector<SegmentState>(state.fieldWidth));
    state.enemyFieldState.resize(state.fieldHeight, std::vector<SegmentState>(state.fieldWidth));
    
    int temp;
    for (int y = 0; y < state.fieldHeight; ++y) {
        for (int x = 0; x < state.fieldWidth; ++x) {
            is >> temp;
            state.userFieldVisible[y][x] = temp;
            is >> temp;
            state.userFieldState[y][x] = static_cast<SegmentState>(temp);
        }
    }
    
    for (int y = 0; y < state.fieldHeight; ++y) {
        for (int x = 0; x < state.fieldWidth; ++x) {
            is >> temp;
            state.enemyFieldVisible[y][x] = temp;
            is >> temp;
            state.enemyFieldState[y][x] = static_cast<SegmentState>(temp);
        }
    }
    
    int size;
    is >> size;
    state.userShipSizes.resize(size);
    for (int i = 0; i < size; ++i) {
        is >> temp;
        state.userShipSizes[i] = static_cast<ShipSize>(temp);
    }
    
    is >> size;
    state.enemyShipSizes.resize(size);
    for (int i = 0; i < size; ++i) {
        is >> temp;
        state.enemyShipSizes[i] = static_cast<ShipSize>(temp);
    }
    
    return is;
} 