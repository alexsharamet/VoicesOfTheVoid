#include "logic/strategy/echo_strategy.h"

namespace Logic {
    std::string EchoStrategy::ask(const PromtHistory &history, const std::string &text) {
        return text;
    };

    std::string EchoStrategy::corrupt(const std::string &text) {
        return text;
    }

    void EchoStrategy::changeWeight(int) {
    }
}// namespace Logic
