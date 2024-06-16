#pragma once

#include <string>

enum class StrategyType {
    AI, Echo, Prepared
};

inline std::string toString(StrategyType type) {
    switch (type) {
        case StrategyType::AI:
            return "AI";
        case StrategyType::Echo:
            return "Echo";
        case StrategyType::Prepared:
            return "Prepared";
    }

    return "UNKNOWN";
}
