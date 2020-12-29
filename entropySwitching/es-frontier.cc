#include "es-frontier.h"

namespace es {
    Frontier::Frontier(std::string feedId, ulong seqNo) {
        this->feedId = feedId;
        this->seqNo = seqNo;   
    }
}