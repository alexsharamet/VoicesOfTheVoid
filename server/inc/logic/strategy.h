#pragma once
#include "promt.h"

#include <memory>

namespace Logic {
    struct IStrategy {
        virtual ~IStrategy() = default;
        virtual std::string ask(std::string text) = 0;
    };

    struct ICorruptionStrategy : public IStrategy {
    public:
        virtual ~ICorruptionStrategy() = default;
        virtual void changeWeight(int weight) = 0;
    };

    enum class StrategyTypes {
        Echo,
        AI
    };

    class EchoStrategy : public ICorruptionStrategy {
    public:
        std::string ask(std::string text) final {
            return text;
        };

        void changeWeight(int) final {
        }
    };
}// namespace Logic