#include "utils/random.h"

#include <cassert>
#include <random>

namespace Utils {
    int random(int min, int max) {
        static std::mt19937 engine(std::random_device{}());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }

    bool isHappened(int possibility /*1-100*/) {
        if (possibility < 1) {
            possibility = 1;
        }
        if (possibility > 100) {
            possibility = 100;
        }
        auto value = random(1, 100);
        return value <= possibility;
    }
}// namespace Utils
