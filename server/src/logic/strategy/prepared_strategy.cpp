#include "logic/strategy/prepared_strategy.h"
#include "utils/random.h"

#include <string>
#include <unordered_map>

namespace Logic {
    std::string PreparedStrategy::ask(const PromtHistory &, const std::string &) {
        static std::unordered_map<int, std::string> prepared = {
                {1, "We must question the story logic of having an all-knowing all-powerful God, who creates faulty Humans, and then blames them for his own mistakes."},
                {2, "If man is to survive, he will have learned to take a delight in the essential differences between men and between cultures. He will learn that differences in ideas and attitudes are a delight, part of life's exciting variety, not something to fear."},
                {3, "It is the struggle itself that is most important. We must strive to be more than we are. It does not matter that we will not reach our ultimate goal. The effort itself yields its own reward."}
        };

        int k = Utils::random(1, prepared.size()) - 1;
        return prepared[k];
    }

    StrategyType PreparedStrategy::getType() const {
        return StrategyType::Prepared;
    }
}// namespace Logic