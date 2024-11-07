#include "./scanner_ability.h"

void Scanner::apply(Field& field, int x, int y) const {
    for (int i = x; i < x + 2 && i < field.getWidth(); ++i) {
        for (int j = y; j < y + 2 && j < field.getHeight(); ++j) {
            field.makePointVisible(i, j);
        }
    }
}