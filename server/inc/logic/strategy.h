#pragma once
#include "promt.h"

#include <memory>

namespace Logic {
    struct IStrategy {
        virtual ~IStrategy() = default;
        virtual std::string ask(const PromtHistory &history, const std::string &text) = 0;
    };

    struct ICorruptionStrategy {
        virtual ~ICorruptionStrategy() = default;
        virtual std::string corrupt(const std::string &text) = 0;
        virtual void changeWeight(int weight) = 0;
    };

    enum class StrategyTypes {
        AI,
        Echo,
        COUNT
    };

    class EchoStrategy
        : public ICorruptionStrategy
        , public IStrategy {
    public:
        std::string ask(const PromtHistory &history, const std::string &text) final {
            return text;
        };

        std::string corrupt(const std::string &text) final {
            return text;
        }

        void changeWeight(int) final {
        }
    };
}// namespace Logic