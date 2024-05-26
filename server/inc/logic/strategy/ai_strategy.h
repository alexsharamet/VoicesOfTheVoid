#pragma once

#include "ai_client/ai.h"
#include "logic/strategy.h"

namespace Logic {
    class AIStrategy : public IStrategy {
    public:
        explicit AIStrategy(std::shared_ptr<AI::AI> ai);
        std::string ask(std::string instruction) final;

    private:
        void addPromt(Promt promt);

    private:
        std::shared_ptr<AI::AI> m_ai;//TODO: make thread safe wrapper
        PromtHistory m_history;
    };
}// namespace Logic