#include "es-meta.h"
namespace es
{

    Meta::Meta(std::string source, ulong seqNo, u_char *verif11, u_char *payloadHash1)
    {
        this->source = source;
        this->seqNo = seqNo;
        memcpy(payloadHash, payloadHash1, 32);
        memcpy(verif1, verif11, 32);
    }

    std::string Meta::serialize()
    {
        return this->getJson().dump();
    }
    
    nlohmann::json Meta::getJson() {
        using json = nlohmann::json;
        json j;
        j["source"] = this->source;
        j["seq_no"] = this->seqNo;
        j["verif_1"] = std::vector<uint8_t>(&this->verif1[0], &this->verif1[32]);
        j["payload_hash"] = std::vector<uint8_t>(&this->payloadHash[0], &this->payloadHash[32]);
        
        return j;
    }

    ulong Meta::getSeqNo() {

        return this->seqNo;
    }

    uint8_t* Meta::getVerif1() {
        return this->verif1;
    }


} // namespace es