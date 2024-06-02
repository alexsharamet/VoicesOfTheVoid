#pragma once

#include "ai_client/ai.h"
#include "logic/strategy.h"

namespace Logic {
    class AIStrategy : public IStrategy {
    public:
        AIStrategy();
        std::string ask(const PromtHistory &history, std::string instruction) final;

    private:
        std::shared_ptr<AI::AI> m_ai;
    };
}// namespace Logic