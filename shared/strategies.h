#pragma once

#include <string>

enum class StrategyType {
    AI, Echo
};

inline std::string toString(StrategyType type) {
    switch (type) {
        case StrategyType::AI:
            return "AI";
        case StrategyType::Echo:
            return "Echo";
    }

    return "UNKNOWN";
}
