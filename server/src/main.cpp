#include "ai_client/ai.h"

#include <iostream>

int main() {
    std::string host = "192.168.1.68";
    int port = 5001;
    std::string promt = "{\"n\":1,\"max_context_length\":2048,\"max_length\":150,\"rep_pen\":1.07,\"temperature\":0.7,\"top_p\":0.92,\"top_k\":100,\"top_a\":0,\"typical\":1,\"tfs\":1,\"rep_pen_range\":320,\"rep_pen_slope\":0.7,\"sampler_order\":[6,0,1,3,4,2,5],\"memory\":\"\",\"genkey\":\"KCPP7178\",\"min_p\":0,\"dynatemp_range\":0,\"dynatemp_exponent\":1,\"smoothing_factor\":0,\"banned_tokens\":[],\"render_special\":false,\"presence_penalty\":0,\"logit_bias\":{},\"prompt\":\"\\n### Instruction:\\nhi\\n### Response:\\n\",\"quiet\":true,\"stop_sequence\":[\"### Instruction:\",\"### Response:\"],\"use_default_badwordsids\":false,\"bypass_eos\":false}";

    AI::AI ai;
    ai.setHost(host);
    ai.setPort(port);

    auto res = ai.sendPromtSync(promt);

    std::cout << res;

    return 0;
}
