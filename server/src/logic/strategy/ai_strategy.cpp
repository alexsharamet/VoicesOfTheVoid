#include <utility>

#include "ext/json.h"
#include "logic/strategy/ai_strategy.h"
#include "utils/config.h"

#include <fmt/core.h>

namespace {
    nlh::json createPromt() {
        nlh::json json;
        json["n"] = 1;
        json["max_context_length"] = 2048;
        json["max_length"] = 150;
        json["rep_pen"] = 1.07;
        json["temperature"] = 0.7;
        json["top_p"] = 0.92;
        json["top_k"] = 100;
        json["top_a"] = 0;
        json["typical"] = 1;
        json["tfs"] = 1;
        json["rep_pen_range"] = 320;
        json["rep_pen_slope"] = 0.7;
        json["sampler_order"] = std::vector<int>{6, 0, 1, 3, 4, 2, 5};
        json["memory"] = "";
        json["genkey"] = "KCPP7178";
        json["min_p"] = 0;
        json["dynatemp_range"] = 0;
        json["dynatemp_exponent"] = 1;
        json["smoothing_factor"] = 0;
        json["banned_tokens"] = nlh::json::array();
        json["render_special"] = false;
        json["presence_penalty"] = 0;
        json["logit_bias"] = nlh::json::object();
        json["quiet"] = true;
        json["stop_sequence"] = std::vector<std::string>{"### Instruction:", "### Response:"};
        json["use_default_badwordsids"] = false;
        json["bypass_eos"] = false;

        return json;
    }
}// namespace

namespace Logic {
    AIStrategy::AIStrategy(std::shared_ptr<AI::AI> ai)
        : m_ai(std::move(ai)) {
    }

    void AIStrategy::addPromt(Promt promt) {
        m_history.push_back(std::move(promt));
        if (m_history.size() > Utils::Config::instance().getHistorySize()) {
            m_history.pop_front();
        }
    }

    std::string AIStrategy::ask(std::string instruction) {
        static nlh::json BASE_PROMT = createPromt();
        nlh::json json(BASE_PROMT);

        std::string promtString;
        for (const auto &promt: m_history) {
            promtString += fmt::format("### Instruction:\n{}\n### Response:\n{}\n", promt.first, promt.second);
        }

        promtString += fmt::format("### Instruction:\n{}\n### Response:\n", instruction);
        json["prompt"] = promtString;

        const auto promt = json.dump();
        auto response = m_ai->sendPromtSync(promt);
        addPromt({instruction, response});
        return response;
    }
}// namespace Logic
