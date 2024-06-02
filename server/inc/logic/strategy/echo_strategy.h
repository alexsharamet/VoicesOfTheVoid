#pragma once
#include "logic/strategy.h"

namespace Logic {
    class EchoStrategy
        : public ICorruptionStrategy
        , public IStrategy {
    public:
        std::string ask(const PromtHistory &history, const std::string &text) final;
        std::string corrupt(const std::string &text) final;
        void changeWeight(int) final;
        StrategyType getType() const final;
    };
}// namespace Logic