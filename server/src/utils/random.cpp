#include "utils/random.h"

#include <random>

namespace Utils {
    int random(int min, int max) {
        static std::mt19937 engine(std::random_device{}());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }

    float randomf(float min, float max) {
        static std::mt19937 engine(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, INT_MAX);
        float number = static_cast<float>(dist(engine)) / static_cast<float>(INT_MAX);//[0..1]

        return min + number * (max - min);
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
