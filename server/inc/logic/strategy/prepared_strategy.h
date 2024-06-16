#pragma once

#include "logic/strategy.h"

namespace Logic {
    class PreparedStrategy : public IStrategy {
        std::string ask(const PromtHistory &history, const std::string &text) final;
        StrategyType getType() const final;
    };
}// namespace Logic