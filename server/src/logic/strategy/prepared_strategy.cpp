#include "logic/strategy/prepared_strategy.h"
#include "utils/random.h"

#include <string>
#include <unordered_map>

namespace Logic {
    std::string PreparedStrategy::ask(const PromtHistory &, const std::string &) {
        static std::unordered_map<int, std::string> prepared = {
                {1, "We must question the story logic of having an all-knowing all-powerful God, who creates faulty Humans, and then blames them for his own mistakes."},
                {2, "If man is to survive, he will have learned to take a delight in the essential differences between men and between cultures. He will learn that differences in ideas and attitudes are a delight, part of life's exciting variety, not something to fear."},
                {3, "It is the struggle itself that is most important. We must strive to be more than we are. It does not matter that we will not reach our ultimate goal. The effort itself yields its own reward."},
                {4, "Reality is incredibly larger, infinitely more exciting, than the flesh and blood vehicle we travel in here. If you read science fiction, the more you read it the more you realize that you and the universe are part of the same thing. Science knows still practically nothing about the real nature of matter, energy, dimension, or time; and even less about those remarkable things called life and thought. But whatever the meaning and purpose of this universe, you are a legitimate part of it. And since you are part of the all that is, part of its purpose, there is more to you than just this brief speck of existence. You \n"
                    "are just a visitor here in this time and this place, a traveler through it."},
                {5, "The Strength of a civilization is not measured by its ability to fight wars, but rather by its ability to prevent them"},
                {6, "I condemn false prophets, I condemn the effort to take away the power of rational decision, to drain people of their free will -- and a hell of a lot of money in the bargain. Religions vary in their degree of idiocy, but I reject them all. For most people, religion is nothing more than a substitute for a malfunctioning brain."},
                {7, "When dreams become more important than reality, you give up travel, building, creating; you even forget how to repair the machines left behind by your ancestors. You just sit living and reliving other lives left behind in the thought records."},
                {8, "Why a journey into space? Because science is now learning that the infinite reaches of our universe probably teem with as much life and adventure as Earth's own oceans and continents. Our galaxy alone is so incredibly vast that the most conservative mathematical odds still add up to millions of planets almost identical to our own — capable of life, even intelligence and strange new civilizations. Alien beings that will range from the fiercely primitive to the incredibly exotic intelligence which will far surpass Mankind."},
                {9, "The human race is a remarkable creature, one with great potential"},
                {10, "Technology would have long ago made privacy impossible, except that this had only made it more precious and desirable--and in the close confines of starship life, respect for another's privacy had become a powerful tradition."},
        };

        int k = Utils::random(1, prepared.size()) - 1;
        return prepared[k];
    }

    StrategyType PreparedStrategy::getType() const {
        return StrategyType::Prepared;
    }
}// namespace Logic