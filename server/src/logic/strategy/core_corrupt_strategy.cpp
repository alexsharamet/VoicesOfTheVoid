#include "logic/strategy/core_corrupt_stategy.h"

#include "utils/random.h"

#include <cassert>
#include <cctype>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {
    static std::unordered_set<char> SPECIAL_SYMBOLS = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '~', '<',
                                                       '>', '?', ',', '.', '{', '}', '[', ']', '`', '\\', '/', '\'', '"',
                                                       '-',
                                                       '=', ';'};

    char getRandomSpecial() {
        auto pos = Utils::random(0, SPECIAL_SYMBOLS.size() - 1);
        auto it = SPECIAL_SYMBOLS.begin();
        std::advance(it, pos);
        return *it;
    }

    bool isSpecialSymbol(char c) {
        return SPECIAL_SYMBOLS.find(c) != SPECIAL_SYMBOLS.end();
    }

    template<typename Match, typename Replace>
    std::string replaceText(const std::string &input, Match match, Replace replace) {
        std::string res;
        for (const char &c: input) {
            res += match(c) ? std::string{replace(c)} : std::string{c};
        }
        return res;
    }

    char shiftChar(char c, int shift) {
        return static_cast<char>(static_cast<int>(c) + shift);
    }

    std::string replaceDigit(const char &c) {
        static std::vector<std::string> digitsWords = {"zero", "one", "two", "three",
                                                       "four", "five", "six", "seven",
                                                       "eight", "nine"};

        assert(std::isdigit(c));
        return digitsWords[static_cast<int>(c - '0')];
    }

    std::string missCharacters(const std::string &input) {
        auto elipsisRand = [] {
            return Utils::random(0, 30);
        };
        auto collapseSizeRand = [] {
            return Utils::random(5, 20);
        };
        std::string res;
        int i = 0;
        auto ellipsis = elipsisRand();
        auto countDown = 0;
        while (i < input.size()) {
            if (countDown < ellipsis) {
                res += input[i];
                ++countDown;
                ++i;
            } else {
                res += "...";
                i += collapseSizeRand();
                ellipsis = elipsisRand();
                countDown = 0;
            }
        }
        return res;
    }

    std::string encode(std::string text) {
        //1. Все числа преобразуются в слова.
        text = replaceText(
                text,
                [](char c) { return static_cast<char>(std::isdigit(c)); },
                replaceDigit);

        //2. Любой символ, такой как точка, дефис, тире и т. д., обязательно заменяется на другой символ или два символа.
        text = replaceText(text,
                           isSpecialSymbol,
                           [](char c) {
                               std::string res;
                               auto times = Utils::random(1, 2);
                               for (auto i = 0; i < times; ++i) {
                                   res += getRandomSpecial();
                               }
                               return res;
                           });

        //3. Для каждой буквы: каждая буква заменяется на букву смещенную на случайное число от 0 до 10 позиций в алфавите.
        /*text = replaceText(
                text,
                [](char c) { return !isSpecialSymbol(c); },
                [](char c) { return shiftChar(c, Utils::random(0, 10)); });*/
        //4. каждый символ имеет вероятность 30% быть замененной на специальный символ.
        text = replaceText(
                text,
                [](char) { return Utils::isHappened(30); },
                [](char c) { return getRandomSpecial(); });

        //5. Каждая буква имеет вероятность 25% стать заглавной.
        text = replaceText(
                text,
                [](char c) { return !isSpecialSymbol(c) && Utils::isHappened(25); },
                [](char c) { return static_cast<char>(std::toupper(c)); });

        return text;
    }

    std::vector<std::string> split(std::string text, const std::string &delimiter) {
        size_t pos = 0;
        std::vector<std::string> res;
        std::string token;
        while ((pos = text.find(delimiter)) != std::string::npos) {
            token = text.substr(0, pos);
            res.push_back(token);
            text.erase(0, pos + delimiter.length());
        }
        return res;
    }

}// namespace

namespace Logic {
    std::string CoreCorruptStrategy::corrupt(const std::string &input) {
        auto data = split(input, " ");
        std::string res;
        for (auto text: data) {
            if (Utils::isHappened(m_weight)) {
                text = encode(text);
            }
            res += text + " ";
        }

        if (m_weight > 0) {
            //6. Каждые либо 10-20 символов вставляются многоточие, при этом удаляется случайное количество символов от 5 до 20.
            res = missCharacters(res);
        }
        return res;
    }

    void CoreCorruptStrategy::changeWeight(int weight) {
        m_weight = weight;
    }
}// namespace Logic