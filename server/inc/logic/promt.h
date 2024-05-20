#pragma once
#include <deque>
#include <string>
#include <utility>

namespace Logic {
    using Promt = std::pair<std::string, std::string>;
    using PromtHistory = std::deque<Promt>;
}// namespace Logic