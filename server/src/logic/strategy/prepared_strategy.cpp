#include "logic/strategy/prepared_strategy.h"
#include "utils/random.h"

#include <string>
#include <unordered_map>

namespace Logic {
    std::string PreparedStrategy::ask(const PromtHistory &, const std::string &) {
        static std::unordered_map<int, std::string> prepared = {
                {1, "one"},
                {2, "two"}};

        int k = Utils::random(1, prepared.size()) - 1;
        return prepared[k];
    }

    StrategyType PreparedStrategy::getType() const {
        return StrategyType::Prepared;
    }
}// namespace Logic