#pragma once

#include "logic/strategy.h"

namespace Logic {
    class CoreCorruptStrategy : public ICorruptionStrategy {
    public:
        std::string corrupt(const std::string &text) override;
        void changeWeight(int weight) override;

    private:
        int m_weight{100};
    };
}// namespace Logic