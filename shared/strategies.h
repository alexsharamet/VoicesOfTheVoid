#pragma once

#include <string>

enum class StrategyType {
    AI, Echo, COUNT
};

std::string toString(StrategyType type) {
    switch (type) {
        case StrategyType::AI:
            return "AI";
        case StrategyType::Echo:
            return "Echo";
        case StrategyType::COUNT:
            return "INVALID";
    }

    return "UNKNOWN";
}
