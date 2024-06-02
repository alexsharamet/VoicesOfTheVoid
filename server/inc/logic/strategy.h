#pragma once
#include "promt.h"

#include <strategies.h>

#include <memory>

namespace Logic {
    struct IStrategy {
        virtual ~IStrategy() = default;
        virtual std::string ask(const PromtHistory &history, const std::string &text) = 0;
        virtual StrategyType getType() const = 0;
    };

    struct ICorruptionStrategy {
        virtual ~ICorruptionStrategy() = default;
        virtual std::string corrupt(const std::string &text) = 0;
        virtual void changeWeight(int weight) = 0;
    };
}// namespace Logic
